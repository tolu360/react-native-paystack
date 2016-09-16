#import <Paystack/Paystack.h>
#import "RCTBridgeModule.h"
#import "RCTBridge.h"
#import "RCTEventDispatcher.h"

@interface RNPaystack : NSObject <RCTBridgeModule>

@property (nonatomic) NSString *errorMsg;
@property (nonatomic) NSString *errorCode;

- (BOOL)isCardNumberValid:(NSString *)cardNumber validateCardBrand:(BOOL)validateCardBrand;
- (BOOL)isExpMonthValid:(NSString *)expMonth;
- (BOOL)isExpYearValid:(NSString *)expYear forMonth:(NSString *)expMonth;
- (BOOL)isCvcValid:(NSString *)cvc withNumber:(NSString *)cardNumber;
- (BOOL)isCardValid:(PSTCKCardParams *)card;
- (BOOL)cardParamsAreValid:(NSString *)cardNumber withMonth:(NSString *)expMonth withYear:(NSString *)expYear andWithCvc:(NSString *)cvc;
- (NSMutableDictionary*)setTokenMsg:(NSString *)token withCardLastDigits:(NSString *)last4;

@end
  