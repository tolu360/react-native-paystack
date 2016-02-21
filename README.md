# React Native Wrapper for Paystack SDK

for Android by [Arttitude 360](http://www.arttitude360.com)

## Index

1. [Description](#1-description)
2. [Installation](#2-installation)
3. [Usage](#3-usage)
4. [Credits](#4-credits)
5. [Changelog](#5-changelog)
6. [License](#6-license)

## 1. Description

This React Native module provides a wrapper to add Paystack Payments to your React Native application using the [Paystack Mobile SDK Android](https://github.com/PaystackHQ/paystack-android) library.

## 2. Installation

You can pull in react-native-paystack via npm:

```shell
npm install react-native-paystack --save
```

### Configuration

#### Step 1 - Update Gradle Settings

```gradle
// file: android/settings.gradle
...

include ':reactnativepaystack'
project(':reactnativepaystack').projectDir = new File(rootProject.projectDir, '../node_modules/react-native-paystack/android')
```

#### Step 2 - Update app Gradle Build

```gradle
// file: android/app/build.gradle
...

dependencies {
    ...
    compile 'com.squareup.okhttp:okhttp:2.5.0'
    compile 'com.squareup.okhttp:okhttp-urlconnection:2.5.0'
    compile project(':reactnativepaystack')
}
```

#### Step 3 - Register React Package

// file: android/app/src/main/java/.../MainActivity

##### react-native < v0.18.0

```java
...
import com.arttitude360.reactnativepaystack.ReactNativePaystackPackage; // <-- import
...

public class MainActivity extends FragmentActivity implements DefaultHardwareBackBtnHandler {

    private ReactInstanceManager mReactInstanceManager;
    private ReactRootView mReactRootView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mReactRootView = new ReactRootView(this);
        mReactInstanceManager = ReactInstanceManager.builder()
                .setApplication(getApplication())
                .setBundleAssetName("index.android.bundle")
                .setJSMainModuleName("index.android")
                .addPackage(new MainReactPackage())
                .addPackage(new ReactNativePaystackPackage()) // <-- Register package here
                .setUseDeveloperSupport(BuildConfig.DEBUG)
                .setInitialLifecycleState(LifecycleState.RESUMED)
                .build();
        mReactRootView.startReactApplication(mReactInstanceManager, "AwesomeProject", null);
        setContentView(mReactRootView);
    }
...
```

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
```
To be more elaborate:

```js
import PaystackAndroid from 'react-native-paystack';

componentDidMount() {
    PaystackAndroid.getToken(
      '4123450131001384', 
      5, 
      2016, 
      883, 
      (resp) => {
        this.setState({error: resp.error});
        console.log(resp);
      },
      (resp) => {
        this.setState({token: resp.token});
        console.log(resp);
      }
    );
  }
```

Explaining the arguments to `PaystackAndroid.getToken`:

+ {Function} successCallback - callback to be invoked on successfully acquiring a token.
 * A single object argument will be passed which has 2 keys: "token" is a string containing the returned token, while "last4" is a string containing the last 4 digits of the card the token belongs to.
+ {Function} errorCallback - callback to be invoked on failure to acquire a valid token.
 * A single object argument will be passed which has 2 keys: "error" is a string containing a description of the error, "code" is an arbitrary error code.
+ cardNumber: the card number as a String without any seperator e.g 5555555555554444
+ expiryMonth: the expiry month as an integer ranging from 1-12 e.g 10 (October)
+ expiryYear: the expiry year as an integer e.g 2015
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

Perhaps needless to say, this module leverages the [Paystack Android SDK](https://github.com/PaystackHQ/paystack-android) for all the heavy liftings.

## 5. CHANGELOG

1.0.11: initial version supporting Android.

## 6. License

 This should be [The MIT License (MIT)](http://www.opensource.org/licenses/mit-license.html). I would have to get back to you on that!

