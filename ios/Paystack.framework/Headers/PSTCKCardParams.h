//
//  PSTCKCardParams.h
//  Paystack
//

#import <Foundation/Foundation.h>
#import "PSTCKFormEncodable.h"
/**
 *  Representation of a user's credit card details. You can assemble these with information that your user enters and
 *  then create Paystack tokens with them using an PSTCKAPIClient. @see https://paystack.com/docs/api#cards
 */
@interface PSTCKCardParams : NSObject<PSTCKFormEncodable>

/**
 *  The card's number.
 */
@property (nonatomic, copy, nullable) NSString *number;

/**
 *  The last 4 digits of the card's number, if it's been set, otherwise nil.
 */
- (nullable NSString *)last4;

/**
 *  The clientdata to send to api.
 */
- (nullable NSString *)clientdata;

/**
 *  The card's expiration month.
 */
@property (nonatomic) NSUInteger expMonth;

/**
 *  The card's expiration year.
 */
@property (nonatomic) NSUInteger expYear;

/**
 *  The card's security code, found on the back.
 */
@property (nonatomic, copy, nullable) NSString *cvc;



@end
