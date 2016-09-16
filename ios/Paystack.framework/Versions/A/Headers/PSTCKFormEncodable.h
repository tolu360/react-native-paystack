//
//  PSTCKFormEncodable.h
//  Paystack
//

#import <Foundation/Foundation.h>

/**
 * Objects conforming to PSTCKFormEncodable can be automatically converted to a form-encoded string, which can then be used when making requests to the Paystack API.
 */
@protocol PSTCKFormEncodable <NSObject>

/**
 * The root object name to be used when converting this object to a form-encoded string. For example, if this returns @"card", then the form-encoded output will resemble @"card[foo]=bar" (where 'foo' and 'bar' are specified by `propertyNamesToFormFieldNamesMapping` below.
 */
+ (nonnull NSString *)rootObjectName;

/**
 * This maps properties on an object that is being form-encoded into parameter names in the Paystack API. For example, PSTCKCardParams has a field called `expMonth`, but the Paystack API expects a field called `exp_month`. This dictionary represents a mapping from the former to the latter (in other words, [PSTCKCardParams propertyNamesToFormFieldNamesMapping][@"expMonth"] == @"exp_month".)
 */
+ (nonnull NSDictionary *)propertyNamesToFormFieldNamesMapping;

/**
 * You can use this property to add additional fields to an API request that are not explicitly defined by the object's interface. This can be useful when using beta features that haven't been added to the Paystack SDK yet. For example, if the /v1/tokens API began to accept a beta field called "test_field", you might do the following:
    PSTCKCardParams *cardParams = [PSTCKCardParams new];
    // add card values
    cardParams.additionalAPIParameters = @{@"test_field": @"example_value"};
    [[PSTCKAPIClient sharedClient] createTokenWithCard:cardParams completion:...];
 */
@property(nonatomic, readwrite, nonnull, copy)NSDictionary *additionalAPIParameters;

@end
