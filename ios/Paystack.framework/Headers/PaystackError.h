//
//  PaystackError.h
//  Paystack
//

#import <Foundation/Foundation.h>

/**
 *  All Paystack iOS errors will be under this domain.
 */
FOUNDATION_EXPORT NSString * __nonnull const PaystackDomain;

typedef NS_ENUM(NSInteger, PSTCKErrorCode) {
    PSTCKConnectionError = 40,          // Trouble connecting to Paystack.
    PSTCKInvalidRequestError = 50,      // Your request had invalid parameters.
    PSTCKAPIError = 60,                 // General-purpose API error (should be rare).
    PSTCKCardError = 70,                // Something was wrong with the given card (most common).
    PSTCKCardErrorProcessingError = 80, // Paystack Checkout encountered an error.
    PSTCKTransactionError = 90,         // Something was wrong with the given transaction details.
    PSTCKConflictError = 100,           // A transaction was started while SDK was processing another
    PSTCKExpiredAccessCodeError = 110,  // The access code is not usable
};

#pragma mark userInfo keys

// A developer-friendly error message that explains what went wrong. You probably
// shouldn't show this to your users, but might want to use it yourself.
FOUNDATION_EXPORT NSString * __nonnull const PSTCKErrorMessageKey;

// What went wrong with your PSTCKCard (e.g., PSTCKInvalidCVC. See below for full list).
FOUNDATION_EXPORT NSString * __nonnull const PSTCKCardErrorCodeKey;

// Which parameter on the PSTCKCard had an error (e.g., "cvc"). Useful for marking up the
// right UI element.
FOUNDATION_EXPORT NSString * __nonnull const PSTCKErrorParameterKey;

#pragma mark PSTCKCardErrorCodeKeys

// (Usually determined locally:)
FOUNDATION_EXPORT NSString * __nonnull const PSTCKInvalidNumber;
FOUNDATION_EXPORT NSString * __nonnull const PSTCKInvalidExpMonth;
FOUNDATION_EXPORT NSString * __nonnull const PSTCKInvalidExpYear;
FOUNDATION_EXPORT NSString * __nonnull const PSTCKInvalidCVC;

// (Usually sent from the server:)
FOUNDATION_EXPORT NSString * __nonnull const PSTCKIncorrectNumber;
FOUNDATION_EXPORT NSString * __nonnull const PSTCKExpiredCard;
FOUNDATION_EXPORT NSString * __nonnull const PSTCKCardDeclined;
FOUNDATION_EXPORT NSString * __nonnull const PSTCKProcessingError;
FOUNDATION_EXPORT NSString * __nonnull const PSTCKIncorrectCVC;

#pragma mark Strings

#define PSTCKExpiredAccessCodeErrorMessage NSLocalizedString(@"There was a problem completing your request", @"Error when access code has no valid transaction")
#define PSTCKCardErrorInvalidNumberUserMessage NSLocalizedString(@"Your card's number is invalid", @"Error when the card number is not valid")
#define PSTCKCardErrorProcessingTransactionMessage NSLocalizedString(@"Please wait", @"Error when chargeCard is called while the SDK is still processing a transaction")
#define PSTCKCardErrorInvalidCVCUserMessage NSLocalizedString(@"Your card's security code is invalid", @"Error when the card's CVC is not valid")
#define PSTCKCardErrorInvalidExpMonthUserMessage                                                                                                                 \
    NSLocalizedString(@"Your card's expiration month is invalid", @"Error when the card's expiration month is not valid")
#define PSTCKCardErrorInvalidExpYearUserMessage                                                                                                                  \
NSLocalizedString(@"Your card's expiration year is invalid", @"Error when the card's expiration year is not valid")
#define PSTCKTransactionErrorDontSetCustomFieldDirectlyMessage                                                                                                                  \
    NSLocalizedString(@"Please use the setCustomField function", @"Error when the app attempts to set the custom_fields key directly")
#define PSTCKCardErrorExpiredCardUserMessage NSLocalizedString(@"Your card has expired", @"Error when the card has already expired")
#define PSTCKCardErrorDeclinedUserMessage NSLocalizedString(@"Your card was declined", @"Error when the card was declined by the credit card networks")
#define PSTCKUnexpectedError                                                                                                                                     \
    NSLocalizedString(@"There was an unexpected error -- try again in a few seconds", @"Unexpected error, such as a 500 from Paystack or a JSON parse error")
#define PSTCKCardErrorProcessingErrorUserMessage                                                                                                                 \
    NSLocalizedString(@"There was an error processing your card -- try again in a few seconds", @"Error when there is a problem processing the credit card")

@interface NSError(Paystack)

+ (nullable NSError *)pstck_errorFromPaystackResponse:(nullable NSDictionary *)jsonDictionary;

@end
