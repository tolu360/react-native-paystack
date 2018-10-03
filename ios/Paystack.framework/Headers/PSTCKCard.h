//
//  PSTCKCard.h
//  Paystack
//

#import <Foundation/Foundation.h>

#import "PSTCKCardBrand.h"
#import "PSTCKCardParams.h"
#import "PSTCKAPIResponseDecodable.h"

/**
 *  The various funding sources for a payment card.
 */
typedef NS_ENUM(NSInteger, PSTCKCardFundingType) {
    PSTCKCardFundingTypeDebit,
    PSTCKCardFundingTypeCredit,
    PSTCKCardFundingTypePrepaid,
    PSTCKCardFundingTypeOther,
};

/**
 *  Representation of a user's credit card details that have been tokenized with the Paystack API. @see https://paystack.com/docs/api#cards
 */
@interface PSTCKCard : PSTCKCardParams<PSTCKAPIResponseDecodable>

/**
 *  The card's number. This will be nil for cards retrieved from the Paystack API.
 */


/**
 *  The last 4 digits of the card.
 */
@property (nonatomic, readonly, nonnull) NSString *last4;

/**
 *  For cards made with Apple Pay, this refers to the last 4 digits of the "Device Account Number" for the tokenized card. For regular cards, it will be nil.
 */
@property (nonatomic, readonly, nullable) NSString *dynamicLast4;

/**
 *  The card's expiration month.
 */
@property (nonatomic) NSUInteger expMonth;

/**
 *  The card's expiration year.
 */
@property (nonatomic) NSUInteger expYear;

/**
 *  The cardholder's name.
 */
@property (nonatomic, copy, nullable) NSString *name;

/**
 *  The cardholder's address.
 */
@property (nonatomic, copy, nullable) NSString *addressLine1;
@property (nonatomic, copy, nullable) NSString *addressLine2;
@property (nonatomic, copy, nullable) NSString *addressCity;
@property (nonatomic, copy, nullable) NSString *addressState;
@property (nonatomic, copy, nullable) NSString *addressZip;
@property (nonatomic, copy, nullable) NSString *addressCountry;

/**
 *  The Paystack ID for the card.
 */
@property (nonatomic, readonly, nullable) NSString *cardId;

/**
 *  The issuer of the card.
 */
@property (nonatomic, readonly) PSTCKCardBrand brand;

/**
 *  The issuer of the card.
 *  Can be one of "Visa", "American Express", "MasterCard", "Discover", "JCB", "Diners Club", or "Unknown"
 *  @deprecated use "brand" instead.
 */
@property (nonatomic, readonly, nonnull) NSString *type __attribute__((deprecated));

/**
 *  The funding source for the card (credit, debit, prepaid, or other)
 */
@property (nonatomic, readonly) PSTCKCardFundingType funding;

/**
 *  A proxy for the card's number, this uniquely identifies the credit card and can be used to compare different cards.
 *  @deprecated This field will no longer be present in responses when using your public key. If you want to access the value of this field, you can look it up on your backend using your secret key.
 */
@property (nonatomic, readonly, nullable) NSString *fingerprint __attribute__((deprecated("This field will no longer be present in responses when using your public key. If you want to access the value of this field, you can look it up on your backend using your secret key.")));

/**
 *  Two-letter ISO code representing the issuing country of the card.
 */
@property (nonatomic, readonly, nullable) NSString *country;

/**
 *  This is only applicable when tokenizing debit cards to issue payouts to managed accounts. You should not set it otherwise. The card can then be used as a transfer destination for funds in this currency.
 */
@property (nonatomic, copy, nullable) NSString *currency;

#pragma mark - deprecated properties

#define DEPRECATED_IN_FAVOR_OF_PSTCKCARDPARAMS __attribute__((deprecated("For collecting your users' credit card details, you should use an PSTCKCardParams object instead of an PSTCKCard.")))

@property (nonatomic, copy, nullable) NSString *number DEPRECATED_IN_FAVOR_OF_PSTCKCARDPARAMS;
@property (nonatomic, copy, nullable) NSString *cvc DEPRECATED_IN_FAVOR_OF_PSTCKCARDPARAMS;
- (void)setExpMonth:(NSUInteger)expMonth DEPRECATED_IN_FAVOR_OF_PSTCKCARDPARAMS;
- (void)setExpYear:(NSUInteger)expYear DEPRECATED_IN_FAVOR_OF_PSTCKCARDPARAMS;
- (void)setName:(nullable NSString *)name DEPRECATED_IN_FAVOR_OF_PSTCKCARDPARAMS;
- (void)setAddressLine1:(nullable NSString *)addressLine1 DEPRECATED_IN_FAVOR_OF_PSTCKCARDPARAMS;
- (void)setAddressLine2:(nullable NSString *)addressLine2 DEPRECATED_IN_FAVOR_OF_PSTCKCARDPARAMS;
- (void)setAddressCity:(nullable NSString *)addressCity DEPRECATED_IN_FAVOR_OF_PSTCKCARDPARAMS;
- (void)setAddressState:(nullable NSString *)addressState DEPRECATED_IN_FAVOR_OF_PSTCKCARDPARAMS;
- (void)setAddressZip:(nullable NSString *)addressZip DEPRECATED_IN_FAVOR_OF_PSTCKCARDPARAMS;
- (void)setAddressCountry:(nullable NSString *)addressCountry DEPRECATED_IN_FAVOR_OF_PSTCKCARDPARAMS;


@end
