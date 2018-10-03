#import <Paystack/Paystack.h>
#import "RNPaystack.h"
#import <React/RCTUtils.h>
#import <React/RCTLog.h>

@implementation RNPaystackModule
{
    RCTPromiseResolveBlock _resolve;
    RCTPromiseRejectBlock _reject;
    NSString *publicKey;
    BOOL requestIsCompleted;
}

- (instancetype)init {
    if ((self = [super init])) {
        requestIsCompleted = YES;
    }
    return self;
}

- (dispatch_queue_t)methodQueue {
    return dispatch_get_main_queue();
}

+ (BOOL)requiresMainQueueSetup
{
    return YES;
}

RCT_EXPORT_MODULE();

- (BOOL)isCardNumberValid:(NSString *)cardNumber validateCardBrand:(BOOL)validateCardBrand
{
    BOOL isValid = ([PSTCKCardValidator validationStateForNumber:cardNumber validatingCardBrand:validateCardBrand] == PSTCKCardValidationStateValid);
    return isValid;
}

- (BOOL)isExpMonthValid:(NSString *)expMonth
{
    BOOL isValid = ([PSTCKCardValidator validationStateForExpirationMonth:expMonth] == PSTCKCardValidationStateValid);
    return isValid;
}

- (BOOL)isExpYearValid:(NSString *)expYear forMonth:(NSString *)expMonth
{
    BOOL isValid = ([PSTCKCardValidator validationStateForExpirationYear:expYear inMonth:expMonth] == PSTCKCardValidationStateValid);
    return isValid;
}

- (BOOL)isCvcValid:(NSString *)cvc withNumber:(NSString *)cardNumber
{
    BOOL isValid = ([PSTCKCardValidator validationStateForCVC:cvc cardBrand:[PSTCKCardValidator brandForNumber:cardNumber]] == PSTCKCardValidationStateValid);
    return isValid;
}

- (BOOL)isCardValid:(PSTCKCardParams *)card
{
    BOOL isValid = ([PSTCKCardValidator validationStateForCard:card] == PSTCKCardValidationStateValid);
    return isValid;
}

- (NSMutableDictionary*)setTokenMsg:(NSString *)token withCardLastDigits:(NSString *)last4
{
    NSMutableDictionary *returnInfo;
    returnInfo = [NSMutableDictionary dictionaryWithCapacity:2];

    [returnInfo setObject:token forKey:@"token"];
    [returnInfo setObject:last4 forKey:@"last4"];

    return returnInfo;
}

- (NSMutableDictionary*)setReferenceMsg:(NSString *)reference
{
    NSMutableDictionary *returnInfo;
    returnInfo = [NSMutableDictionary dictionaryWithCapacity:1];

    [returnInfo setObject:reference forKey:@"reference"];

    return returnInfo;
}

- (BOOL)cardParamsAreValid:(NSString *)cardNumber withMonth:(NSString *)expMonth withYear:(NSString *)expYear andWithCvc:(NSString *)cvc
{
    if (! [self isCardNumberValid:cardNumber validateCardBrand:YES]) {
        self.errorMsg = @"Invalid card number";
        self.errorCode = @"E_INVALID_NUMBER";
        return NO;
    }

    if (! [self isExpMonthValid:expMonth]) {
        self.errorMsg = @"Invalid expiration month";
        self.errorCode = @"E_INVALID_MONTH";
        return NO;
    }

    if (! [self isExpYearValid:expYear forMonth:expMonth]) {
        self.errorMsg = @"Invalid expiration year";
        self.errorCode = @"E_INVALID_YEAR";
        return NO;
    }

    if (! [self isCvcValid:cvc withNumber:cardNumber]) {
        self.errorMsg = @"Invalid CVC";
        self.errorCode = @"E_INVALID_CVC";
        return NO;
    }

    return YES;

}

RCT_EXPORT_METHOD(init:(NSDictionary *)options) {
    publicKey = options[@"publicKey"];
    [Paystack setDefaultPublicKey:publicKey];
}

RCT_EXPORT_METHOD(chargeCard:(NSDictionary *)params 
    resolver:(RCTPromiseResolveBlock)resolve rejecter:(RCTPromiseRejectBlock)reject)
{
    _resolve = resolve;
    _reject = reject;
    UIViewController *rootViewController = [UIApplication sharedApplication].delegate.window.rootViewController;

    if(!requestIsCompleted) {
        _reject(@"E_BUSY", @"Another request is still being processed, please wait", nil);
        return;
    }

    requestIsCompleted = NO;
    
    if (! [self cardParamsAreValid:params[@"cardNumber"] withMonth:params[@"expiryMonth"] withYear:params[@"expiryYear"] andWithCvc:params[@"cvc"]]) {

        // NSMutableDictionary *returnInfo = [self setErrorMsg:self.errorMsg withErrorCode:self.errorCode];
        if (_reject) {
            _reject(self.errorCode, self.errorMsg, nil);
        }

    } else {

        PSTCKCardParams *cardParams = [[PSTCKCardParams alloc] init];
        cardParams.number = params[@"cardNumber"];
        cardParams.expMonth = [params[@"expiryMonth"] integerValue];
        cardParams.expYear = [params[@"expiryYear"] integerValue];
        cardParams.cvc = params[@"cvc"];

        PSTCKTransactionParams *transactionParams = [[PSTCKTransactionParams alloc] init];
        transactionParams.amount = [params[@"amountInKobo"] integerValue];
        transactionParams.email = params[@"email"];

        if (params[@"currency"] != nil) {
            transactionParams.currency = params[@"currency"];
        }

        if (params[@"plan"] != nil) {
            transactionParams.plan = params[@"plan"];
        }            

        if (params[@"subAccount"] != nil) {
            transactionParams.subaccount = params[@"subAccount"];

            if (params[@"bearer"] != nil) {
                transactionParams.bearer = params[@"bearer"];
            }

            if (params[@"transactionCharge"] != nil) {
                transactionParams.transaction_charge = [params[@"transactionCharge"] integerValue];
            }
        }

        if (params[@"reference"] != nil) {
            transactionParams.reference = params[@"reference"];
        }            

        if ([self isCardValid:cardParams]) {
            
            [[PSTCKAPIClient sharedClient] chargeCard:cardParams
                            forTransaction:transactionParams
                        onViewController:rootViewController
                            didEndWithError:^(NSError *error, NSString *reference){
                                            requestIsCompleted = YES;
                                            if (_reject) {
                                                _reject(@"E_CHARGE_ERROR", @"Error charging card", error);
                                            }
                                        }
                        didRequestValidation: ^(NSString *reference){
                                            // an OTP was requested, transaction has not yet succeeded
                                            NSLog(@"- RNPaystack: an OTP was requested, transaction has not yet succeeded");
                                        }
                    didTransactionSuccess: ^(NSString *reference){
                                            requestIsCompleted = YES;
                                            // transaction may have succeeded, please verify on server
                                            NSLog(@"- RNPaystack: transaction may have succeeded, please verify on server");
                                            NSMutableDictionary *returnInfo = [self setReferenceMsg:reference];

                                            if (_resolve) {
                                                _resolve(returnInfo);
                                            }
            }];

        } else {
            requestIsCompleted = YES;

            if (_reject) {
                _reject(@"E_INVALID_CARD", @"Card is invalid", nil);
            }
        }
    
    }     
}

RCT_EXPORT_METHOD(chargeCardWithAccessCode:(NSDictionary *)params 
    resolver:(RCTPromiseResolveBlock)resolve rejecter:(RCTPromiseRejectBlock)reject)
{
    _resolve = resolve;
    _reject = reject;
    UIViewController *rootViewController = [UIApplication sharedApplication].delegate.window.rootViewController;

    if(!requestIsCompleted) {
        _reject(@"E_BUSY", @"Another request is still being processed, please wait", nil);
        return;
    }

    requestIsCompleted = NO;
    
    if (! [self cardParamsAreValid:params[@"cardNumber"] withMonth:params[@"expiryMonth"] withYear:params[@"expiryYear"] andWithCvc:params[@"cvc"]]) {

        // NSMutableDictionary *returnInfo = [self setErrorMsg:self.errorMsg withErrorCode:self.errorCode];
        if (_reject) {
            _reject(self.errorCode, self.errorMsg, nil);
        }

    } else {

        PSTCKCardParams *cardParams = [[PSTCKCardParams alloc] init];
        cardParams.number = params[@"cardNumber"];
        cardParams.expMonth = [params[@"expiryMonth"] integerValue];
        cardParams.expYear = [params[@"expiryYear"] integerValue];
        cardParams.cvc = params[@"cvc"];

        PSTCKTransactionParams *transactionParams = [[PSTCKTransactionParams alloc] init];
        transactionParams.access_code = params[@"accessCode"];            

        if ([self isCardValid:cardParams]) {
            
            [[PSTCKAPIClient sharedClient] chargeCard:cardParams
                            forTransaction:transactionParams
                        onViewController:rootViewController
                            didEndWithError:^(NSError *error, NSString *reference){
                                            requestIsCompleted = YES;
                                            if (_reject) {
                                                _reject(@"E_CHARGE_ERROR", @"Error charging card", error);
                                            }
                                        }
                        didRequestValidation: ^(NSString *reference){
                                            // an OTP was requested, transaction has not yet succeeded
                                            NSLog(@"- RNPaystack: an OTP was requested, transaction has not yet succeeded");
                                        }
                    didTransactionSuccess: ^(NSString *reference){
                                            requestIsCompleted = YES;
                                            // transaction may have succeeded, please verify on server
                                            NSLog(@"- RNPaystack: transaction may have succeeded, please verify on server");
                                            NSMutableDictionary *returnInfo = [self setReferenceMsg:reference];

                                            if (_resolve) {
                                                _resolve(returnInfo);
                                            }
            }];

        } else {
            requestIsCompleted = YES;

            if (_reject) {
                _reject(@"E_INVALID_CARD", @"Card is invalid", nil);
            }
        }
    
    }

}

@end
  