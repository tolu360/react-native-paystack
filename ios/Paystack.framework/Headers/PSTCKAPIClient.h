//
//  PSTCKAPIClient.h
//  PaystackExample
//

#import <Foundation/Foundation.h>
#if TARGET_OS_IPHONE
#import <UIKit/UIViewController.h>
#endif

static NSString *const __nonnull PSTCKSDKVersion = @"3.0.5";
static NSString *const __nonnull PSTCKSDKBuild = @"14";

@class PSTCKCard, PSTCKCardParams, PSTCKTransactionParams, PSTCKToken;

/**
 *  A callback to be run with a token response from the Paystack API.
 *
 *  @param reference The Transaction reference from the response. Will be nil if an error occurs.
 *  @param error The error returned from the response, or nil in one occurs. @see PaystackError.h for possible values.
 */
typedef void (^PSTCKErrorCompletionBlock)(NSError * __nonnull error, NSString * __nullable reference);
typedef void (^PSTCKTransactionCompletionBlock)(NSString * __nonnull reference);
typedef void (^PSTCKNotifyCompletionBlock)(void);

/**
 A top-level class that imports the rest of the Paystack SDK. This class used to contain several methods to create Paystack tokens, but those are now deprecated in
 favor of PSTCKAPIClient.
 */
@interface Paystack : NSObject

/**
 *  Set your Paystack API key with this method. New instances of PSTCKAPIClient will be initialized with this value. You should call this method as early as
 *  possible in your application's lifecycle, preferably in your AppDelegate.
 *
 *  @param   publicKey Your public key, obtained from https://paystack.com/account/apikeys
 *  @warning Make sure not to ship your test API keys to the App Store! This will log a warning if you use your test key in a release build.
 */
+ (void)setDefaultPublicKey:(nonnull NSString *)publicKey;

/// The current default public key.
+ (nullable NSString *)defaultPublicKey;
@end

/// A client for making connections to the Paystack API.
@interface PSTCKAPIClient : NSObject

/**
 *  A shared singleton API client. Its API key will be initially equal to [Paystack defaultPublicKey].
 */
+ (nonnull instancetype)sharedClient;
- (nonnull instancetype)initWithPublicKey:(nonnull NSString *)publicKey NS_DESIGNATED_INITIALIZER;

/**
 *  @see [Paystack setDefaultPublicKey:]
 */
@property (nonatomic, copy, nullable) NSString *publicKey;

/**
 *  The operation queue on which to run completion blocks passed to the api client. Defaults to [NSOperationQueue mainQueue].
 */
@property (nonatomic, nonnull) NSOperationQueue *operationQueue;

@end

#pragma mark Credit Cards

@interface PSTCKAPIClient (CreditCards)

/**
 *  Charges a PSTCKCardParams object using the Paystack API.
 *
 *  @param card        The user's card details. Cannot be nil. @see https://paystack.com/docs/api#create_card_token
 */
- (void)      chargeCard:(nonnull PSTCKCardParams *)card
          forTransaction:(nonnull PSTCKTransactionParams *)transaction
        onViewController:(nonnull UIViewController *)viewController
         didEndWithError:(nonnull PSTCKErrorCompletionBlock)errorCompletion
    didRequestValidation:(nonnull PSTCKTransactionCompletionBlock)beforeValidateCompletion
   didTransactionSuccess:(nonnull PSTCKTransactionCompletionBlock)successCompletion;

- (void)      chargeCard:(nonnull PSTCKCardParams *)card
          forTransaction:(nonnull PSTCKTransactionParams *)transaction
        onViewController:(nonnull UIViewController *)viewController
         didEndWithError:(nonnull PSTCKErrorCompletionBlock)errorCompletion
    didRequestValidation:(nonnull PSTCKTransactionCompletionBlock)beforeValidateCompletion
       willPresentDialog:(nonnull PSTCKNotifyCompletionBlock)showingDialogCompletion
         dismissedDialog:(nonnull PSTCKNotifyCompletionBlock)dialogDismissedCompletion
   didTransactionSuccess:(nonnull PSTCKTransactionCompletionBlock)successCompletion;

- (void)      chargeCard:(nonnull PSTCKCardParams *)card
          forTransaction:(nonnull PSTCKTransactionParams *)transaction
        onViewController:(nonnull UIViewController *)viewController
         didEndWithError:(nonnull PSTCKErrorCompletionBlock)errorCompletion
       willPresentDialog:(nonnull PSTCKNotifyCompletionBlock)showingDialogCompletion
         dismissedDialog:(nonnull PSTCKNotifyCompletionBlock)dialogDismissedCompletion
   didTransactionSuccess:(nonnull PSTCKTransactionCompletionBlock)successCompletion;

@end
