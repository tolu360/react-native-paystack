# React Native Wrapper for Paystack Mobile SDKs

for Android & iOS by [Arttitude 360](http://www.arttitude360.com)

## Index

1. [Description](#1-description)
2. [Installation](#2-installation)
3. [Usage](#3-usage)
4. [Credits](#4-credits)
5. [Changelog](#5-changelog)
6. [License](#6-license)

## 1. Description

This React Native module provides a wrapper to add Paystack Payments to your React Native application using the [Paystack Android Mobile SDK](https://github.com/PaystackHQ/paystack-android) and the [Paystack iOS Mobile SDK](https://github.com/PaystackHQ/paystack-ios) libraries.

## 2. Installation

You can pull in react-native-paystack via npm:

```shell
npm install react-native-paystack --save
```
OR 

```shell
yarn add react-native-paystack
```

### Versioning
For `RN <=0.39` use version 2+ e.g. react-native-paystack@2.1.4
For `RN >=0.40` use version 3+ e.g. react-native-paystack@3.0.4

### Configuration

#### Automatic (iOS & Android)

```shell
react-native link react-native-paystack 
```
- (iOS only): The next steps are necessary for iOS at this time as publishing to NPM seems to break symlinks contained in the Paystack iOS framework shipped with this package, thus causing XCode build errors.
- Download a fresh copy of the `Paystack iOS framework` from [Dropbox](https://www.dropbox.com/s/rxds20w3ud4wqs2/PaystackiOS%20%283%29.zip?dl=0) or from their [releases page on Github](https://github.com/PaystackHQ/paystack-ios/releases/).
- Extract `Paystack.framework` from the downloaded zip.
- In XCode's "Project navigator", right click on project name folder ➜ `Add Files to <Your-Project-Name>`. Ensure `Copy items if needed` and `Create groups` are checked and select your copy of `Paystack.framework`.
- Your files tree in XCode should look similar to the screenshot below:

<img width=200 title="XCode files tree" src="./file-tree.png">

- If you are working with XCode 8+, to allow encryptions work properly with the Paystack SDK, you may need to enable `Keychain Sharing` for your app. In the Capabilities pane, if Keychain Sharing isn’t enabled, toggle ON the switch in the Keychain Sharing section.

<img width=400 title="XCode files tree" src="./4_enablekeychain_2x.png">

#### Manual Config (iOS)

- The following steps are optional, should be taken if you have not run `react-native link react-native-paystack` already.
- In XCode's "Project navigator", right click on project name folder ➜ `Add Files to <...>`. Ensure `Copy items if needed` and `Create groups` are checked
- Go to `node_modules` ➜ `react-native-paystack/ios` ➜ add `RNPaystack.xcodeproj`.

#### Manual Config (Android)

- The following steps are optional, should be taken if you have not run `react-native link react-native-paystack` already.
- Add the following in your `android/settings.gradle` file:

```java
include ':react-native-paystack'
project(':react-native-paystack').projectDir = new File(rootProject.projectDir, '../node_modules/react-native-paystack/android')
```
- Add the following in your `android/app/build.grade` file:

```java
dependencies {
    ...
    compile project(':react-native-paystack')
}
```
- Add the following in your `...MainApplication.java` file:

```java
import com.arttitude360.reactnative.rnpaystack.RNPaystackPackage;

@Override
protected List<ReactPackage> getPackages() {
  return Arrays.<ReactPackage>asList(
      new MainReactPackage(),
      ...
      new RNPaystackPackage() //<-- Add line
  );
}
``` 

## 3. Usage

### Import Library
- For ios, edit your `AppDelegate.m` file and import the Paystack framework:

```Objective-C
#import <Paystack/Paystack.h>
...
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
  ...
  
  [Paystack setDefaultPublishableKey:@"INSERT-PUBLIC-KEY-HERE"];
  ...

}
```

- For Android, add the following tag in your `android/app/src/main/AndroidManifest.xml` file:

```xml
  <meta-data android:name="co.paystack.android.PublishableKey" android:value="INSERT-PUBLIC-KEY-HERE"/>
```

### Getting a Token (iOS & Android - To be deprecated soon)
It's a cinch to obtain a single-use card token using the react-native-paystack module. Pls note, the SDK assumes you are responsible for building the card form/UI.

```javascript
	RNPaystack.getToken(cardParams);
```
To be more elaborate, `cardParams` is a Javascript `Object` representing the card to be tokenized and `RNPaystack.getToken()` returns a Javascript `Promise` like:

```js
import RNPaystack from 'react-native-paystack';

getToken() {

	RNPaystack.getToken({
      cardNumber: '4123450131001381', 
      expiryMonth: '10', 
      expiryYear: '17', 
      cvc: '883'
    })
	.then(response => {
	  console.log(response); // do stuff with the token
	})
	.catch(error => {
	  console.log(error); // error is a javascript Error object
	  console.log(error.message);
	  console.log(error.code);
	})
	
}
```

#### Request Signature

| Argument        | Type           | Description  |
| ------------- |:-------------:| :-----|
| cardNumber          | string | the card number as a String without any seperator e.g 5555555555554444 |
| expiryMonth      | string      | the card expiry month as a double-digit ranging from 1-12 e.g 10 (October) |
| expiryYear | string      | the card expiry year as a double-digit e.g 15 |
| cvc | string | the card 3/4 digit security code as a String e.g 123 |

#### Response Object

An object of the form is returned from a successful token request

```javascript
{
	token: "PSTK_4aw6i0yizwvyzjx",
	last4: "1381"
}
```


### Charging the tokens. 
Send the token to your server and create a charge by calling the Paystack REST API. An authorization_code will be returned once the single-use token has been charged successfully. You can learn more about the Paystack API [here](https://developers.paystack.co/docs/getting-started).
 
 **Endpoint:** https://api.paystack.co/transaction/charge_token

 **Parameters:** 

 - email  - customer's email address (required)
 - reference - unique reference  (required)
 - amount - Amount in Kobo (required) 

**Example**

```bash
    curl https://api.paystack.co/transaction/charge_token \
    -H "Authorization: Bearer SECRET_KEY" \
    -H "Content-Type: application/json" \
    -d '{"token": "PSTK_r4ec2m75mrgsd8n9", "email": "customer@email.com", "amount": 10000, "reference": "amutaJHSYGWakinlade256"}' \
    -X POST

```

### Charging a Card (iOS & Android)
Using the react-native-paystack module, you can complete the transaction with the Paystack Android and iOS SDKs. Note that as with getting a card token, the SDK assumes you are responsible for building the card form/UI.

```javascript
RNPaystack.chargeCard(chargeParams);
```
To be more elaborate, `chargeParams` is a Javascript `Object` representing the parameters of the charge to be initiated and `RNPaystack.chargeCard()` returns a Javascript `Promise` like:

```js
import RNPaystack from 'react-native-paystack';

chargeCard() {

	RNPaystack.chargeCard({
      cardNumber: '4123450131001381', 
      expiryMonth: '10', 
      expiryYear: '17', 
      cvc: '883',
      email: 'chargeIOS@master.dev',
      amountInKobo: 150000,
      subAccount: 'ACCT_pz61jjjsslnx1d9',
    })
	.then(response => {
	  console.log(response); // card charged successfully, get reference here
	})
	.catch(error => {
	  console.log(error); // error is a javascript Error object
	  console.log(error.message);
	  console.log(error.code);
	})
	
}
```

#### Request Signature (chargeParams)

| Argument        | Type           | Description  |
| ------------- |:-------------:| :-----|
| cardNumber          | string | the card number as a String without any seperator e.g 5555555555554444 |
| expiryMonth      | string      | the card expiry month as a double-digit ranging from 1-12 e.g 10 (October) |
| expiryYear | string      | the card expiry year as a double-digit e.g 15 |
| cvc | string | the card 3/4 digit security code as e.g 123 |
| email | string | email of the user to be charged |
| amountInKobo | integer | the transaction amount in kobo |
| currency (optional) | string | sets the currency for the transaction e.g. USD |
| plan (optional) | string | sets the plan ID if the transaction is to create a subscription e.g. PLN_n0p196bg73y4jcx |
| subAccount (optional) | string | sets the subaccount ID for split-payment transactions e.g. ACCT_pz61jjjsslnx1d9 |
| transactionCharge (optional) | integer | the amount to be charged on a split-payment, use only when `subAccount` is set |
| bearer (optional) | string | sets which party bears paystack fees on a split-payment e.g. 'subaccount', use only when `subAccount` is set |
| reference (optional) | string | sets the transaction reference which must be unique per transaction |

#### Response Object

An object of the form is returned from a successful charge

```javascript
{
	reference: "trx_1k2o600w"
}
```

### Verifying a Charge
Verify a charge by calling Paystack's [REST API](https://api.paystack.co/transaction/verify) with the `reference` obtained above. An `authorization_code` will be returned once the card has been charged successfully. Learn more about that [here](https://developers.paystack.co/docs/verify-transaction).

 **Parameter:** 

 - reference  - the transaction reference (required)

 **Example**

 ```bash
 $ curl https://api.paystack.co/transaction/verify/trx_1k2o600w \
    -H "Authorization: Bearer SECRET_KEY" \
    -H "Content-Type: application/json" \
    -X GET
 ```

## 4. CREDITS

Perhaps needless to say, this module leverages the [Paystack Android SDK](https://github.com/PaystackHQ/paystack-android) and the [Paystack IOS SDK](https://github.com/PaystackHQ/paystack-ios) for all the heavy liftings.

## 5. CHANGELOG

+ 1.0.12: Initial version supporting Android.
+ 1.1.1: Android library upgrade and initial iOS support.
+ 2.0.0: A couple of breaking changes have been introduced, see [Old Docs](./Old Docs.md) for previous documentations.
+ 2.0.0: Upgraded to v2.0 of the Paystack Android SDK.
+ 2.0.0: Unified APIs across both platforms (iOS & Android).
+ 2.0.0: Methods now return Javascript Promises on both platforms.
+ 2.1.1: Upgraded to v2.1+ of both the Paystack iOS and Android SDKs.
+ 2.1.1: Added support for `chargeCard` on both platforms.
+ 2.1.1: Added support for `subscriptions` and `split-payments`.
+ 2.1.1: For v2-specific documentations, see [v2 Docs](./v2-Docs.md)

## 6. License

 This should be [The MIT License (MIT)](http://www.opensource.org/licenses/mit-license.html). I would have to get back to you on that!

