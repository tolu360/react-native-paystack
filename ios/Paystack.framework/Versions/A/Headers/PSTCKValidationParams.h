//
//  PSTCKValidationParams.h
//  Paystack
//

#import <Foundation/Foundation.h>
#import "PSTCKFormEncodable.h"
/**
 *  Representation of a user's credit card details. You can assemble these with information that your user enters and
 *  then create Paystack tokens with them using an PSTCKAPIClient. @see https://paystack.com/docs/api#cards
 */
@interface PSTCKValidationParams : NSObject<PSTCKFormEncodable>

@property (nonatomic, copy, nonnull) NSString *trans;
@property (nonatomic, copy, nonnull) NSString *token;

@end
