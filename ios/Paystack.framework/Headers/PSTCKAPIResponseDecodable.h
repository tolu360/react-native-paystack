//
//  PSTCKAPIResponseDecodable.h
//  Paystack
//

#import <Foundation/Foundation.h>

@protocol PSTCKAPIResponseDecodable <NSObject>

/**
 * These fields are required to be present in the API response. If any of them are nil, decodedObjectFromAPIResponse should also return nil.
 */
+ (nonnull NSArray *)requiredFields;

/**
 * Parses an response from the Paystack API (in JSON format; represented as an NSDictionary) into an instance of the class. Returns nil if the object could not be decoded (i.e. if one of its `requiredFields` is nil).
 */
+ (nullable instancetype)decodedObjectFromAPIResponse:(nonnull NSDictionary *)response;

/**
 * The raw JSON response used to create the object. This can be useful for using beta features that haven't yet been made into properties in the SDK.
 */
@property(nonatomic, readonly, nonnull, copy)NSDictionary *allResponseFields;

@end
