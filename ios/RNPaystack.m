#import <Paystack/Paystack.h>
#import "RNPaystack.h"
#import "RCTUtils.h"
#import "RCTLog.h"

@implementation RNPaystack
{
    RCTPromiseResolveBlock _resolve;
    RCTPromiseRejectBlock _reject;
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

RCT_EXPORT_METHOD(getToken:(NSString *)rawNumber withExpiryMonth:(NSString *)rawExpMonth withExpiryYear:(NSString *)rawExpYear andCvc:(NSString *)rawCvc 
    resolver:(RCTPromiseResolveBlock)resolve rejecter:(RCTPromiseRejectBlock)reject)
{
    _resolve = resolve;
    _reject = reject;

    dispatch_async(dispatch_get_main_queue(), ^{
        if (! [self cardParamsAreValid:rawNumber withMonth:rawExpMonth withYear:rawExpYear andWithCvc:rawCvc]) {

            // NSMutableDictionary *returnInfo = [self setErrorMsg:self.errorMsg withErrorCode:self.errorCode];
            if (_reject) {
                _reject(self.errorCode, self.errorMsg, nil);
            }

        } else {
            PSTCKCardParams *cardParam = [[PSTCKCardParams alloc] init];
            cardParam.number = rawNumber;
            cardParam.expMonth = [rawExpMonth integerValue];
            cardParam.expYear = [rawExpYear integerValue];
            cardParam.cvc = rawCvc;

            if ([self isCardValid:cardParam]) {
                [[PSTCKAPIClient sharedClient] createTokenWithCard:cardParam completion:^(PSTCKToken *token, NSError *error) {
                    if (token) {
                        NSLog(@"- RNPaystack Token is set");
                        
                        NSMutableDictionary *returnInfo = [self setTokenMsg:token.tokenId withCardLastDigits:token.last4];
                        NSLog(@"token is set: %@", token.tokenId);
                        
                        NSLog(@"- RNPaystack responseCallback is set");
                        if (_resolve) {
                            _resolve(returnInfo);
                        }
                    }

                    if (error) {
                        NSLog(@"- RNPaystack TokenError is set");
                        // NSMutableDictionary *returnInfo = [self setErrorMsg:@"Error retrieving token for card." withErrorCode:401];

                        if (_reject) {
                            _reject(@"E_TOKEN_ERROR", @"Error retrieving token for card", error);
                        }
                    }
                }];
            } else {
                // NSMutableDictionary *returnInfo = [self setErrorMsg:@"Invalid Card." withErrorCode:404];

                if (_reject) {
                    _reject(@"E_INVALID_CARD", @"Card is invalid", nil);
                }
            }
        
        }
       
    });
}

@end
  