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

#### IOS

1. In XCode's "Project navigator", right click on project name folder ➜ `Add Files to <...>`
 - Ensure `Copy items if needed` and `Create groups` are checked
2. Go to `node_modules` ➜ `react-native-paystack` ➜ add `paystack-ios` folder
3. To be sure you are all set, manually inspect your project target settings - Ensure:
  + path to `paystack-ios` folder is set in `Build Settings > Search Paths > Framework Search Paths`
  + `PaystackIOS.m` is listed in `Build Phases > Compile Sources`
  + `Paystack.framework` is listed in `Build Phases > Link Binary with Libraries`. 
4. Edit your `AppDelegate.m` and import the Paystack framework:

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
*Compile and have some card tokens!*


#### Android

#### Step 1 - Update Gradle Settings

```gradle
// file: android/settings.gradle
...

include ':reactnativepaystack'
project(':reactnativepaystack').projectDir = new File(rootProject.projectDir, '../node_modules/react-native-paystack/paystack-android')
```

#### Step 2 - Update app Gradle Build

```gradle
// file: android/app/build.gradle
...

dependencies {
    ...
    compile project(':reactnativepaystack')
}
```

#### Step 3 - Register React Package

// file: android/app/src/main/java/.../MainActivity

##### react-native >= v0.18.0

```java
import com.arttitude360.reactnativepaystack.ReactNativePaystackPackage;  // <-- import
...
/**
   * A list of packages used by the app. If the app uses additional views
   * or modules besides the default ones, add more packages here.
   */
    @Override
    protected List<ReactPackage> getPackages() {
      return Arrays.<ReactPackage>asList(
        new MainReactPackage(),
        new ReactNativePaystackPackage());  // <-- Register package here
    }
```


Add the following tag in your `android/app/src/main/AndroidManifest.xml` file:

```xml
  <meta-data android:name="co.paystack.android.PublishableKey" android:value="INSERT-PUBLIC-KEY-HERE"/>
```

## 3. Usage

### Getting a Token
It's a cinch to obtain a single-use token with the PaystackSdk using the react-native-paystack module.

```js
PaystackAndroid.getToken(cardNumber, expiryMonth, expiryYear, cvc, errorCallback, successCallback);
PaystackIOS.getToken(cardNumber, expiryMonth, expiryYear, cvc, responseCallback);
```
To be more elaborate:

#### On Android

```js
import PaystackAndroid from 'react-native-paystack';

componentDidMount() {
    PaystackAndroid.getToken(
      '4123450131001384', 
      '05', 
      '16', 
      '883', 
      (resp) => {
        // didn't get a token - something went wrong
        this.setState({error: resp.error});
        console.log(resp);
      },
      (resp) => {
        // got a token - do your thang!
        this.setState({token: resp.token});
        console.log(resp);
      }
    );
  }
```

#### On iOS

```js
import PaystackIOS from 'react-native-paystack';

componentDidMount() {
    PaystackIOS.getToken(
      '4123450131001384', 
      '05', 
      '16', 
      '883', 
      (error, resp) => {
        if (error) {
          // didn't get a token - something went wrong
          this.setState({errorMsg: error.error});
        }
        if (resp) {
          // got a token - do your thang!
          this.setState({tokenStr: resp.token});
        }
      }
    );
  }
```

Explaining the arguments to `PaystackAndroid.getToken`:

+ {Function} responseCallback (only on iOS) - callback to be invoked with the result of calling `getToken` - takes in 2 arguments - (error, response). If any error is set, you get an error object with 2 keys: "error" is a string containing a description of the error, "code" is an arbitrary error code.
If a token is returned, you get a response object with 2 keys: "token" is a string containing the returned token, while "last4" is a string containing the last 4 digits of the card the token belongs to.
+ {Function} successCallback (only on android) - callback to be invoked on successfully acquiring a token.
 * A single object argument will be passed which has 2 keys: "token" is a string containing the returned token, while "last4" is a string containing the last 4 digits of the card the token belongs to.
+ {Function} errorCallback (only on android) - callback to be invoked on failure to acquire a valid token.
 * A single object argument will be passed which has 2 keys: "error" is a string containing a description of the error, "code" is an arbitrary error code.
+ cardNumber: the card number as a String without any seperator e.g 5555555555554444
+ expiryMonth: the expiry month as an integer ranging from 1-12 e.g 10 (October)
+ expiryYear: the expiry year as an integer e.g 15 (2 digits - very !important for iOS)
+ cvc: the card security code as a String e.g 123
+ To make it easy for you, you can pass all string or all integer arguments - the module will handle the type castings for you.

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


## 4. CREDITS

Perhaps needless to say, this module leverages the [Paystack Android SDK](https://github.com/PaystackHQ/paystack-android) and the [Paystack IOS SDK](https://github.com/PaystackHQ/paystack-ios) for all the heavy liftings.

## 5. CHANGELOG

+ 1.0.12: initial version supporting Android.
+ 1.1.1: android library upgrade and initial iOS support.

## 6. License

 This should be [The MIT License (MIT)](http://www.opensource.org/licenses/mit-license.html). I would have to get back to you on that!

