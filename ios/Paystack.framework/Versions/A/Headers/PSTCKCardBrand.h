//
//  PSTCKCardBrand.h
//  Paystack
//

#import <Foundation/Foundation.h>

/**
 *  The various card brands to which a payment card can belong.
 */
typedef NS_ENUM(NSInteger, PSTCKCardBrand) {
    PSTCKCardBrandVisa,
    PSTCKCardBrandAmex,
    PSTCKCardBrandMasterCard,
    PSTCKCardBrandDiscover,
    PSTCKCardBrandJCB,
    PSTCKCardBrandDinersClub,
    PSTCKCardBrandUnknown,
    PSTCKCardBrandVerve,
};
