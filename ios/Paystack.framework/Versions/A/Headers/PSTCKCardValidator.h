//
//  PSTCKCardValidator.h
//  Paystack
//

#import <Foundation/Foundation.h>

#import "PSTCKCardParams.h"
#import "PSTCKCardBrand.h"
#import "PSTCKCardValidationState.h"

/**
 *  This class contains static methods to validate card numbers, expiration dates, and CVCs. For a list of test card numbers to use with this code, see https://paystack.com/docs/testing
 */
@interface PSTCKCardValidator : NSObject

/**
 *  Returns a copy of the passed string with all non-numeric characters removed.
 */
+ (nonnull NSString *)sanitizedNumericStringForString:(nonnull NSString *)string;

/**
 *  Whether or not the target string contains only numeric characters.
 */
+ (BOOL)stringIsNumeric:(nonnull NSString *)string;

/**
 *  Validates a card number, passed as a string. This will return PSTCKCardValidationStateInvalid for numbers that are too short or long, contain invalid characters, do not pass Luhn validation, or (optionally) do not match a number format issued by a major card brand.
 *
 *  @param cardNumber The card number to validate. Ex. @"4242424242424242"
 *  @param validatingCardBrand Whether or not to enforce that the number appears to be issued by a major card brand (or could be). For example, no issuing card network currently issues card numbers beginning with the digit 9; if an otherwise correct-length and luhn-valid card number beginning with 9 (example: 9999999999999995) were passed to this method, it would return PSTCKCardValidationStateInvalid if this parameter were YES and PSTCKCardValidationStateValid if this parameter were NO. If unsure, you should use YES for this value.
 *
 *  @return PSTCKCardValidationStateValid if the number is valid, PSTCKCardValidationStateInvalid if the number is invalid, or PSTCKCardValidationStateIncomplete if the number is a substring of a valid card (e.g. @"4242").
 */
+ (PSTCKCardValidationState)validationStateForNumber:(nonnull NSString *)cardNumber validatingCardBrand:(BOOL)validatingCardBrand;

/**
 *  The card brand for a card number or substring thereof.
 *
 *  @param cardNumber A card number, or partial card number. For example, @"4242", @"5555555555554444", or @"123".
 *
 *  @return The brand for that card number. The example parameters would return PSTCKCardBrandVisa, PSTCKCardBrandMasterCard, and PSTCKCardBrandUnknown, respectively.
 */
+ (PSTCKCardBrand)brandForNumber:(nonnull NSString *)cardNumber;

/**
 *  The number length for cards associated with a card brand. For example, Visa card numbers contain 16 characters, while American Express cards contain 15 characters.
 */
+ (NSUInteger)lengthForCardBrand:(PSTCKCardBrand)brand;

/**
 *  The length of the final grouping of digits to use when formatting a card number for display. For example, Visa cards display their final 4 numbers, e.g. "4242", while American Express cards display their final 5 digits, e.g. "10005".
 */
+ (NSInteger)fragmentLengthForCardBrand:(PSTCKCardBrand)brand;

/**
 *  Validates an expiration month, passed as an (optionally 0-padded) string. Example valid values are "3", "12", and "08". Example invalid values are "99", "a", and "00". Incomplete values include "0" and "1".
 *
 *  @param expirationMonth A string representing a 2-digit expiration month for a payment card.
 *
 *  @return PSTCKCardValidationStateValid if the month is valid, PSTCKCardValidationStateInvalid if the month is invalid, or PSTCKCardValidationStateIncomplete if the month is a substring of a valid month (e.g. @"0" or @"1").
 */
+ (PSTCKCardValidationState)validationStateForExpirationMonth:(nonnull NSString *)expirationMonth;

/**
 *  Validates an expiration year, passed as a string representing the final 2 digits of the year. This considers the period between the current year until 2099 as valid times. An example valid value would be "16" (assuming the current year, as determined by [NSDate date], is 2015). Will return PSTCKCardValidationStateInvalid for a month/year combination that is earlier than the current date (i.e. @"15" and @"04" in October 2015. Example invalid values are "00", "a", and "13". Any 1-digit string will return PSTCKCardValidationStateIncomplete.
 *
 *  @param expirationYear A string representing a 2-digit expiration year for a payment card.
 *  @param expirationMonth A string representing a 2-digit expiration month for a payment card. See -validationStateForExpirationMonth for the desired formatting of this string.
 *
 *  @return PSTCKCardValidationStateValid if the year is valid, PSTCKCardValidationStateInvalid if the year is invalid, or PSTCKCardValidationStateIncomplete if the year is a substring of a valid year (e.g. @"1" or @"2").
 */
+ (PSTCKCardValidationState)validationStateForExpirationYear:(nonnull NSString *)expirationYear inMonth:(nonnull NSString *)expirationMonth;

/**
 *  The max CVC length for a card brand (for context, American Express CVCs are 4 digits, while all others are 3).
 */
+ (NSUInteger)maxCVCLengthForCardBrand:(PSTCKCardBrand)brand;

/**
 *  Validates a card's CVC, passed as a numeric string, for the given card brand.
 *
 *  @param cvc   the CVC to validate
 *  @param brand the card brand (can be determined from the card's number using +brandForNumber)
 *
 *  @return Whether the CVC represents a valid CVC for that card brand. For example, would return PSTCKCardValidationStateValid for @"123" and PSTCKCardBrandVisa, PSTCKCardValidationStateValid for @"1234" and PSTCKCardBrandAmericanExpress, PSTCKCardValidationStateIncomplete for @"12" and PSTCKCardBrandVisa, and PSTCKCardValidationStateInvalid for @"12345" and any brand.
 */
+ (PSTCKCardValidationState)validationStateForCVC:(nonnull NSString *)cvc cardBrand:(PSTCKCardBrand)brand;

/**
 *  Validates the given card details.
 *
 *  @param card the card details to validate.
 * 
 *  @return PSTCKCardValidationStateValid if all fields are valid, PSTCKCardValidationStateInvalid if any field is invalid, or PSTCKCardValidationStateIncomplete if all fields are either incomplete or valid.
 */
+ (PSTCKCardValidationState)validationStateForCard:(nonnull PSTCKCardParams *)card;

// Exposed for testing only.
+ (PSTCKCardValidationState)validationStateForExpirationYear:(nonnull NSString *)expirationYear inMonth:(nonnull NSString *)expirationMonth inCurrentYear:(NSInteger)currentYear currentMonth:(NSInteger)currentMonth;
+ (PSTCKCardValidationState)validationStateForCard:(nonnull PSTCKCardParams *)card inCurrentYear:(NSInteger)currentYear currentMonth:(NSInteger)currentMonth;

@end
