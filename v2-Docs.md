## Use the API below for v2 of this package only.
- Download a fresh copy of the `Paystack iOS framework` from [Dropbox](https://www.dropbox.com/s/ykt5h0xjjkfwmk6/Paystack.framework.zip?dl=0) to continue to work with v2.0.0.

### Getting a Token (iOS & Android)
It's a cinch to obtain a single-use card token using the react-native-paystack module. Pls note, the SDK assumes you are responsible for building the card form/UI.

```javascript
RNPaystack.getToken(cardNumber, expiryMonth, expiryYear, cvc);
```
To be more elaborate, `RNPaystack.getToken()` returns a Javascript `Promise` like:

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

### Charging a Card (Android Only)
Using the react-native-paystack module, you can complete the transaction with the Paystack Android SDK. Note that as with getting a card token, the SDK assumes you are responsible for building the card form/UI.

```javascript
RNPaystack.chargeCard(cardNumber, expiryMonth, expiryYear, cvc, email, amountInKobo);
```
To be more elaborate, `RNPaystack.chargeCard()` returns a Javascript `Promise` like:

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