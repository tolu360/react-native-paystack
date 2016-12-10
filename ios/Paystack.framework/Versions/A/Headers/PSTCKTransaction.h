//
//  PSTCKTransaction.h
//  Paystack
//

#import <Foundation/Foundation.h>
#import "PSTCKAPIResponseDecodable.h"


/**
 *  A transaction returned from submitting payment details to the Paystack API. You should not have to instantiate one of these directly.
 */
@interface PSTCKTransaction : NSObject<PSTCKAPIResponseDecodable>

/**
 *  You cannot directly instantiate an PSTCKTransaction. You should only use one that has been returned from an PSTCKAPIClient callback.
 */
- (nonnull instancetype) init __attribute__((unavailable("You cannot directly instantiate an PSTCKTransaction. You should only use one that has been returned from an PSTCKAPIClient callback.")));

@property (nonatomic, readonly, nonnull) NSString *reference;
@property (nonatomic, readonly, nonnull) NSString *message;
@property (nonatomic, readonly, nonnull) NSString *status;
@property (nonatomic, readonly, nonnull) NSString *trans;
@property (nonatomic, readonly, nonnull) NSString *redirecturl;


@end
