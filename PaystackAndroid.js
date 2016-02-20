/**
* @providesModule PaystackAndroid
*/

'use strict';

var { NativeModules } = require('react-native');

class PaystackAndroid {
	getToken(cardNumber, expiryMonth, expiryYear, cvc, errorCb, successCb) {
		NativeModules.PaystackAndroid.getToken(String(cardNumber), parseInt(expiryMonth), parseInt(expiryYear), String(cvc), errorCb, successCb);
	}
}

module.exports = PaystackAndroid;