#import <Paystack/Paystack.h>
#import <React/RCTBridgeModule.h>
#import <React/RCTBridge.h>
#import <React/RCTEventDispatcher.h>

@interface RNPaystackModule : NSObject <RCTBridgeModule>

@property (nonatomic) NSString *errorMsg;
@property (nonatomic) NSString *errorCode;

- (BOOL)isCardNumberValid:(NSString *)cardNumber validateCardBrand:(BOOL)validateCardBrand;
- (BOOL)isExpMonthValid:(NSString *)expMonth;
- (BOOL)isExpYearValid:(NSString *)expYear forMonth:(NSString *)expMonth;
- (BOOL)isCvcValid:(NSString *)cvc withNumber:(NSString *)cardNumber;
- (BOOL)isCardValid:(PSTCKCardParams *)card;
- (BOOL)cardParamsAreValid:(NSString *)cardNumber withMonth:(NSString *)expMonth withYear:(NSString *)expYear andWithCvc:(NSString *)cvc;
- (NSMutableDictionary*)setTokenMsg:(NSString *)token withCardLastDigits:(NSString *)last4;
- (NSMutableDictionary*)setReferenceMsg:(NSString *)reference;

@end
  