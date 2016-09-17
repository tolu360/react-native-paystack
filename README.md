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

### Configuration

#### Automatic (iOS & Android)

```shell
react-native link react-native-paystack 
```
- (iOS only): The next steps are necessary for iOS at this time as publishing to NPM seems to break symlinks contained in the Paystack iOS framework shipped with this package, thus causing XCode build errors.
- Download a fresh copy of the `Paystack iOS framework` from [Dropbox](https://www.dropbox.com/s/ykt5h0xjjkfwmk6/Paystack.framework.zip?dl=0) or from their [releases page on Github](https://github.com/PaystackHQ/paystack-ios/releases/).
- Extract `Paystack.framework` from the downloaded zip.
- In XCode's "Project navigator", right click on project name folder ➜ `Add Files to <Your-Project-Name>`. Ensure `Copy items if needed` and `Create groups` are checked and select your copy of `Paystack.framework`.
- Your files tree in XCode should look similar to the screenshot below:

<img width=200 title="XCode files tree" src="./file-tree.png">

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
import com.arttitude360.reactnative.rngpaystack.RNPaystackPackage;

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

### Getting a Token (iOS & Android)
It's a cinch to obtain a single-use card token using the react-native-paystack module. Pls note, the SDK assumes you are responsible for building the card form/UI.

```javascript
RNPaystackAndroid.getToken(cardNumber, expiryMonth, expiryYear, cvc);
```
To be more elaborate, `RNPaystackAndroid.getToken()` returns a `Promise` like:

```js
import RNPaystack from 'react-native-paystack';

getToken() {

	RNPaystack.getToken('4123450131001381', '01', '17', '883')
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

### Charging a Card (Android Only)
Using the react-native-paystack module, you can complete the transaction with the Paystack Android SDK. Note that as with getting a card token, the SDK assumes you are responsible for building the card form/UI.

```javascript
RNPaystackAndroid.chargeCard(cardNumber, expiryMonth, expiryYear, cvc, email, amountInKobo);
```
To be more elaborate, `RNPaystackAndroid.chargeCard()` returns a `Promise` like:

```js
import RNPaystack from 'react-native-paystack';

chargeCard() {

	RNPaystack.chargeCard('4123450131001381', '01', '17', '883', 'dev-master@rnpaystack.dev', '10000')
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

#### Request Signature

| Argument        | Type           | Description  |
| ------------- |:-------------:| :-----|
| cardNumber          | string | the card number as a String without any seperator e.g 5555555555554444 |
| expiryMonth      | string      | the card expiry month as a double-digit ranging from 1-12 e.g 10 (October) |
| expiryYear | string      | the card expiry year as a double-digit e.g 15 |
| cvc | string | the card 3/4 digit security code as e.g 123 |
| email | string | email of the user to be charged |
| amountInKobo | integer | the transaction amount in kobo |

#### Response Object

An object of the form is returned from a successful charge

```javascript
{
	reference: "trx_1k2o600w"
}
```

## 4. CREDITS

Perhaps needless to say, this module leverages the [Paystack Android SDK](https://github.com/PaystackHQ/paystack-android) and the [Paystack IOS SDK](https://github.com/PaystackHQ/paystack-ios) for all the heavy liftings.

## 5. CHANGELOG

+ 1.0.12: initial version supporting Android.
+ 1.1.1: android library upgrade and initial iOS support.
+ 2.0.0: A couple of breaking changes have been introduced, see [Old Docs](./Old Docs.md) for previous documentations.
+ 2.0.0: Upgraded to v2.0 of the Paystack Android SDK.
+ 2.0.0: Unified APIs across both platforms (iOS & Android).
+ 2.0.0: Methods now return Javascript Promises on both platforms.

## 6. License

 This should be [The MIT License (MIT)](http://www.opensource.org/licenses/mit-license.html). I would have to get back to you on that!

