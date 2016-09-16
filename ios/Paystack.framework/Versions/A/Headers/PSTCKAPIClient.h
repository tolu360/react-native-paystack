//
//  PSTCKAPIClient.h
//  PaystackExample
//

#import <Foundation/Foundation.h>

static NSString *const __nonnull PSTCKSDKVersion = @"1.0.0";

@class PSTCKCard, PSTCKCardParams, PSTCKToken;

/**
 *  A callback to be run with a token response from the Paystack API.
 *
 *  @param token The Paystack token from the response. Will be nil if an error occurs. @see PSTCKToken
 *  @param error The error returned from the response, or nil in one occurs. @see PaystackError.h for possible values.
 */
typedef void (^PSTCKTokenCompletionBlock)(PSTCKToken * __nullable token, NSError * __nullable error);

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

@end

#pragma mark - Deprecated Methods

/**
 *  A callback to be run with a token response from the Paystack API.
 *
 *  @param token The Paystack token from the response. Will be nil if an error occurs. @see PSTCKToken
 *  @param error The error returned from the response, or nil in one occurs. @see PaystackError.h for possible values.
 *  @deprecated This has been renamed to PSTCKTokenCompletionBlock.
 */
typedef void (^PSTCKCompletionBlock)(PSTCKToken * __nullable token, NSError * __nullable error) __attribute__((deprecated("PSTCKCompletionBlock has been renamed to PSTCKTokenCompletionBlock.")));

// These methods are deprecated. You should instead use PSTCKAPIClient to create tokens.
// Example: [Paystack createTokenWithCard:card completion:completion];
// becomes [[PSTCKAPIClient sharedClient] createTokenWithCard:card completion:completion];
@interface Paystack (Deprecated)

/**
 *  Securely convert your user's credit card details into a Paystack token, which you can then safely store on your server and use to charge the user. The URL
 *connection will run on the main queue. Uses the value of [Paystack defaultPublishableKey] for authentication.
 *
 *  @param card    The user's card details. @see PSTCKCard
 *  @param handler Code to run when the user's card has been turned into a Paystack token.
 *  @deprecated    Use PSTCKAPIClient instead.
 */
+ (void)createTokenWithCard:(nonnull PSTCKCard *)card completion:(nullable PSTCKCompletionBlock)handler __attribute__((deprecated));

/**
 *  Securely convert your user's credit card details into a Paystack token, which you can then safely store on your server and use to charge the user. The URL
 *connection will run on the main queue.
 *
 *  @param card           The user's card details. @see PSTCKCard
 *  @param publishableKey The API key to use to authenticate with Paystack. Get this at https://paystack.com/account/apikeys .
 *  @param handler        Code to run when the user's card has been turned into a Paystack token.
 *  @deprecated           Use PSTCKAPIClient instead.
 */
+ (void)createTokenWithCard:(nonnull PSTCKCard *)card publishableKey:(nonnull NSString *)publishableKey completion:(nullable PSTCKCompletionBlock)handler __attribute__((deprecated));

/**
 *  Securely convert your user's credit card details into a Paystack token, which you can then safely store on your server and use to charge the user.
 *
 *  @param card    The user's card details. @see PSTCKCard
 *  @param queue   The operation queue on which to run completion blocks passed to the api client. 
 *  @param handler Code to run when the user's card has been turned into a Paystack token.
 *  @deprecated    Use PSTCKAPIClient instead.
 */
+ (void)createTokenWithCard:(nonnull PSTCKCard *)card operationQueue:(nonnull NSOperationQueue *)queue completion:(nullable PSTCKCompletionBlock)handler __attribute__((deprecated));

/**
 *  Securely convert your user's credit card details into a Paystack token, which you can then safely store on your server and use to charge the user.
 *
 *  @param card           The user's card details. @see PSTCKCard
 *  @param publishableKey The API key to use to authenticate with Paystack. Get this at https://paystack.com/account/apikeys .
 *  @param queue          The operation queue on which to run completion blocks passed to the api client. 
 *  @param handler        Code to run when the user's card has been turned into a Paystack token.
 *  @deprecated           Use PSTCKAPIClient instead.
 */
+ (void)createTokenWithCard:(nonnull PSTCKCard *)card
             publishableKey:(nonnull NSString *)publishableKey
             operationQueue:(nonnull NSOperationQueue *)queue
                 completion:(nullable PSTCKCompletionBlock)handler __attribute__((deprecated));

@end
