//
//  PSTCKAPIClient.h
//  PaystackExample
//

#import <Foundation/Foundation.h>
#if TARGET_OS_IPHONE
#import <UIKit/UIViewController.h>
#endif

static NSString *const __nonnull PSTCKSDKVersion = @"2.2.0";

@class PSTCKCard, PSTCKCardParams, PSTCKTransactionParams, PSTCKToken;

/**
 *  A callback to be run with a token response from the Paystack API.
 *
 *  @param token The Paystack token from the response. Will be nil if an error occurs. @see PSTCKToken
 *  @param error The error returned from the response, or nil in one occurs. @see PaystackError.h for possible values.
 */
typedef void (^PSTCKTokenCompletionBlock)(PSTCKToken * __nullable token, NSError * __nullable error);
typedef void (^PSTCKErrorCompletionBlock)(NSError * __nonnull error);
typedef void (^PSTCKTransactionCompletionBlock)(NSString * __nonnull reference);

/**
 A top-level class that imports the rest of the Paystack SDK. This class used to contain several methods to create Paystack tokens, but those are now deprecated in
 favor of PSTCKAPIClient.
 */
@interface Paystack : NSObject

/**
 *  Set your Paystack API key with this method. New instances of PSTCKAPIClient will be initialized with this value. You should call this method as early as
 *  possible in your application's lifecycle, preferably in your AppDelegate.
 *
 *  @param   publishableKey Your publishable key, obtained from https://paystack.com/account/apikeys
 *  @warning Make sure not to ship your test API keys to the App Store! This will log a warning if you use your test key in a release build.
 */
+ (void)setDefaultPublishableKey:(nonnull NSString *)publishableKey;

/// The current default publishable key.
+ (nullable NSString *)defaultPublishableKey;
@end

/// A client for making connections to the Paystack API.
@interface PSTCKAPIClient : NSObject

/**
 *  A shared singleton API client. Its API key will be initially equal to [Paystack defaultPublishableKey].
 */
+ (nonnull instancetype)sharedClient;
- (nonnull instancetype)initWithPublishableKey:(nonnull NSString *)publishableKey NS_DESIGNATED_INITIALIZER;

/**
 *  @see [Paystack setDefaultPublishableKey:]
 */
@property (nonatomic, copy, nullable) NSString *publishableKey;

/**
 *  The operation queue on which to run completion blocks passed to the api client. Defaults to [NSOperationQueue mainQueue].
 */
@property (nonatomic, nonnull) NSOperationQueue *operationQueue;

@end

#pragma mark Credit Cards

@interface PSTCKAPIClient (CreditCards)

/**
 *  Converts an PSTCKCardParams object into a Paystack token using the Paystack API.
 *
 *  @param card        The user's card details. Cannot be nil. @see https://paystack.com/docs/api#create_card_token
 *  @param completion  The callback to run with the returned Paystack token (and any errors that may have occurred).
 */
- (void)createTokenWithCard:(nonnull PSTCKCardParams *)card completion:(nullable PSTCKTokenCompletionBlock)completion;

- (void)      chargeCard:(nonnull PSTCKCardParams *)card
          forTransaction:(nonnull PSTCKTransactionParams *)transaction
        onViewController:(nonnull UIViewController *)viewController
         didEndWithError:(nonnull PSTCKErrorCompletionBlock)errorCompletion
    didRequestValidation:(nullable PSTCKTransactionCompletionBlock)beforeValidateCompletion
   didTransactionSuccess:(nonnull PSTCKTransactionCompletionBlock)successCompletion;

@end
