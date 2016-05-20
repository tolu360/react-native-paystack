//
//  PSTCKToken.h
//  Paystack
//

#import <Foundation/Foundation.h>
#import "PSTCKAPIResponseDecodable.h"


/**
 *  A token returned from submitting payment details to the Paystack API. You should not have to instantiate one of these directly.
 */
@interface PSTCKToken : NSObject<PSTCKAPIResponseDecodable>

/**
 *  You cannot directly instantiate an PSTCKToken. You should only use one that has been returned from an PSTCKAPIClient callback.
 */
- (nonnull instancetype) init __attribute__((unavailable("You cannot directly instantiate an PSTCKToken. You should only use one that has been returned from an PSTCKAPIClient callback.")));

/**
 *  The value of the token. You can store this value on your server and use it to make charges and customers. @see
 * https://paystack.com/docs/mobile/ios#sending-tokens
 */
@property (nonatomic, readonly, nonnull) NSString *tokenId;
@property (nonatomic, readonly, nonnull) NSString *message;
@property (nonatomic, readonly, nonnull) NSString *last4;


/**
 *  When the token was created.
 */

@end
