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

### PS: If you are using this library in production, please give the repo a star - I am not being vain and have no interest in the vanity metric, just trying to figure out if it is still worth the time or effort spent supporting the library. Cheers!

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
- For `RN >=0.40` only;
- Breaking Change Alert for v3.2.0+. Looking for the docs for the 3.1.* version of this library? [Check here!](./README-3-1.md)

### Configuration

#### Post-Install Steps (iOS)

##### 1) Auto Linking & Cocoapods Integration (React Native 0.59 and lower)
- If you do not have CocoaPods already installed on your machine, run `gem install cocoapods` to set it up the first time. (Hint: Go grab a cup of coffee!)
- If you are not using Cocoapods in your project already, run `cd ios && pod init` at the root directory of your project. This would create a `Podfile` in your `ios` directory.
- Run `react-native link react-native-paystack` at the root directory of your project and ensure you edit your Podfile to look like the sample below (remove all the targets you are not building for, such as Tests and tvOS):

```ruby
# platform :ios, '9.0'

target '_YOUR_PROJECT_TARGET_' do

  # Pods for _YOUR_PROJECT_TARGET_
  pod 'React', :path => '../node_modules/react-native', :subspecs => [
    'Core',
    'CxxBridge',
    'DevSupport',
    'RCTText',
    'RCTImage',
    'RCTNetwork',
    'RCTWebSocket',
    'RCTSettings',
    'RCTAnimation',
    'RCTLinkingIOS',
    # Add any other subspecs you want to use in your project
    # Remove any subspecs you don't want to use in your project
  ]

  pod "yoga", :path => "../node_modules/react-native/ReactCommon/yoga"
  pod 'DoubleConversion', :podspec => '../node_modules/react-native/third-party-podspecs/DoubleConversion.podspec'
  pod 'glog', :podspec => '../node_modules/react-native/third-party-podspecs/glog.podspec'
  pod 'Folly', :podspec => '../node_modules/react-native/third-party-podspecs/Folly.podspec'
  # This should already be auto-added for you, if not add the line below
  pod 'react-native-paystack', :path => '../node_modules/react-native-paystack'

end
```

- Replace all references to _YOUR_PROJECT_TARGET_ with your project target (it's the same as project name by default).
- By now, you should be all set to install the packages from your Podfile. Run `pod install` from your `ios` directory.
- Close Xcode, and then open (double-click) your project's .xcworkspace file to launch Xcode. From this time onwards, you must use the `.xcworkspace` file to open the project. Or just use the `react-native run-ios` command as usual to run your app in the simulator.

##### 2) Auto Linking & Cocoapods Integration (React Native 0.60 and higher)
Since React Native 0.60 and higher, [autolinking](https://github.com/react-native-community/cli/blob/master/docs/autolinking.md) makes the installation process simpler.

```sh
cd ios
pod install
```
- Close Xcode, and then open (double-click) your project's .xcworkspace file to launch Xcode. From this time onwards, you must use the `.xcworkspace` file to open the project. Or just use the `react-native run-ios` command as usual to run your app in the simulator.

#### Manual Config (iOS)

- The following steps are optional, should be taken if you have not run `react-native link react-native-paystack` already.
- In XCode's "Project navigator", right click on project name folder ➜ `Add Files to <...>`. Ensure `Copy items if needed` and `Create groups` are checked
- Go to `node_modules` ➜ `react-native-paystack/ios` ➜ add `RNPaystack.xcodeproj`.
- Click on your main project file (the one that represents the .xcodeproj for your project) select `Build Phases` and drag the static library, `libRNPaystack.a` from the `Products` folder inside `RNPaystack.xcodeproj` to `Link Binary With Libraries`. See the [react-native docs](https://facebook.github.io/react-native/docs/linking-libraries-ios.html) for more details.

#### Autolinking on Android (React Native 0.59 and lower)
- Run `react-native link react-native-paystack` at the root directory of your project.

#### Autolinking on Android (React Native 0.60 and higher)
Since React Native 0.60 and higher, [autolinking](https://github.com/react-native-community/cli/blob/master/docs/autolinking.md) makes the installation process simpler. Nothing more to do here (Gradle has you all set to go) - just head over to usage!

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

#### More Config (Only applicable to Android using react-native-paystack v3.1.4+ & RN less than 0.57.0)
- Update Gradle plugin to v3.0.0+ for your app, follow the following steps if you are not sure how:
  * Edit your `~ android/build.gradle` to look similar to [build.gradle](https://github.com/tolu360/vestarapp/blob/master/android/build.gradle)
  * Edit your `~ android/gradle/wrapper/gradle-wrapper.properties` to look similar to [gradle-wrapper.properties](https://github.com/tolu360/vestarapp/blob/master/android/gradle/wrapper/gradle-wrapper.properties)
- To avoid build issues, enable `Aapt2` for your project by adding `android.enableAapt2=true` to your `android/gradle.properties` file.
- If you are using RN with a version lower than 0.57.0, it is important you replace your `node-modules/react-native/react.gradle` file with [this version @ commit da6a5e0](https://github.com/facebook/react-native/blob/da6a5e0439c168147271ef66ad5ebbeebd6fce3b/react.gradle) to avoid further build issues when assembling a release version of your app.

## 3. Usage

### Initialize Library
Somewhere high up in your project and way before calling any other method exposed by this library, your `index` file or equivalent is a good spot, ensure you initialize the library with your `public key` as follos:

```js
import RNPaystack from 'react-native-paystack';

RNPaystack.init({ publicKey: 'YOUR_PUBLIC_KEY_HERE' });
```

### Charging a Card with Access Code (iOS & Android)
It's a cinch to charge a card token using the react-native-paystack module. This is the recommended or the most-preferred workflow favored by the folks at Paystack. Initiate a new transaction on your server side using the appropriate [Paystack endpoint](https://developers.paystack.co/reference#initialize-a-transaction) - obtain an `access_code` and complete the charge on your mobile application. Pls note, the SDK assumes you are responsible for building the card form/UI.

```javascript
	RNPaystack.chargeCardWithAccessCode(cardParams);
```
To be more elaborate, `cardParams` is a Javascript `Object` representing the card to be charged and `RNPaystack.chargeCardWithAccessCode()` returns a Javascript `Promise` like:

```js
import RNPaystack from 'react-native-paystack';

chargeCard() {

	RNPaystack.chargeCardWithAccessCode({
      cardNumber: '4123450131001381', 
      expiryMonth: '10', 
      expiryYear: '17', 
      cvc: '883',
      accessCode: '2p3j42th639duy4'
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
| accessCode | string | the access_code obtained for the charge |

#### Response Object

An object of the form is returned from a successful token request

```javascript
{
	reference: "trx_1k2o600w"
}
```

### Charging a Card (iOS & Android)
Using the react-native-paystack module, you can start and complete a transaction with the mobile Paystack Android and iOS SDKs. With this option, you pass both your charge and card properties to the SDK - with this worklow, you initiate and complete a transaction on your mobile app. Note that as with charging with an access_code, the SDK assumes you are responsible for building the card form/UI.

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
+ 3.1.0: Retired support for `getToken` on both platforms.
+ 3.1.0: Added support for `chargeCardWithAccessCode` on both platforms.
+ 3.1.0: Upgraded to v3.*+ of both the Paystack iOS and Android SDKs.
+ 3.1.1: Fix for breaking change in RN v0.47+
* 3.1.4: Miscellaneous and dependencies update on Android.
* 3.2.0: A Breaking Change - Initialize library in JS, rather than in native code.
* 3.3.0: Move to a CocoaPods Flow for iOS.

## 6. License

 This should be [The MIT License (MIT)](http://www.opensource.org/licenses/mit-license.html). I would have to get back to you on that!

