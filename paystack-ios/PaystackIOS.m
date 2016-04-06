/********* PaystackIOS.m Plugin Implementation *******/

#import "PaystackIOS.h"



@implementation PaystackIOS

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

- (NSMutableDictionary*)setErrorMsg:(NSString *)errorMsg withErrorCode:(int)errorCode
{
    NSMutableDictionary *returnInfo;
    returnInfo = [NSMutableDictionary dictionaryWithCapacity:2];

    [returnInfo setObject:errorMsg forKey:@"error"];
    [returnInfo setObject:[NSNumber numberWithInt:errorCode] forKey:@"code"];

    return returnInfo;
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
        self.errorMsg = @"Invalid card number.";
        self.errorCode = 421;
        return NO;
    }

    if (! [self isExpMonthValid:expMonth]) {
        self.errorMsg = @"Invalid expiration month.";
        self.errorCode = 424;
        return NO;
    }

    if (! [self isExpYearValid:expYear forMonth:expMonth]) {
        self.errorMsg = @"Invalid expiration year.";
        self.errorCode = 425;
        return NO;
    }

    if (! [self isCvcValid:cvc withNumber:cardNumber]) {
        self.errorMsg = @"Invalid cvc code.";
        self.errorCode = 423;
        return NO;
    }

    return YES;

}

RCT_EXPORT_METHOD(getToken:(NSString *)rawNumber withExpiryMonth:(NSString *)rawExpMonth withExpiryYear:(NSString *)rawExpYear andCvc:(NSString *)rawCvc sendResponse:(RCTResponseSenderBlock)responseCallback)
{
    self.responseCallback = responseCallback;

    dispatch_async(dispatch_get_main_queue(), ^{
        if (! [self cardParamsAreValid:rawNumber withMonth:rawExpMonth withYear:rawExpYear andWithCvc:rawCvc]) {

            NSMutableDictionary *returnInfo = [self setErrorMsg:self.errorMsg withErrorCode:self.errorCode];

            if (self.responseCallback) {
                self.responseCallback(@[returnInfo]);
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
                        NSLog(@"- PaystackIOS Token is set");
                        
                        NSMutableDictionary *returnInfo = [self setTokenMsg:token.tokenId withCardLastDigits:token.last4];
                        NSLog(@"token is set: %@", token.tokenId);
                        
                        NSLog(@"- PaystackIOS responseCallback is set");
                        if (self.responseCallback) {
                            self.responseCallback(@[[NSNull null], returnInfo]);
                        }
                    }

                    if (error) {
                        NSLog(@"- PaystackIOS TokenError is set");
                        NSMutableDictionary *returnInfo = [self setErrorMsg:@"Error retrieving token for card." withErrorCode:401];

                        if (self.responseCallback) {
                            self.responseCallback(@[returnInfo]);
                        }
                    }
                }];
            } else {
                NSMutableDictionary *returnInfo = [self setErrorMsg:@"Invalid Card." withErrorCode:404];

                if (self.responseCallback) {
                    self.responseCallback(@[returnInfo]);
                }
            }
        
        }
       
    });
}

@end