/**
* @providesModule PaystackIOS
*/

'use strict';

var { NativeModules } = require('react-native');

var PaystackIOS = {
	getToken: function(cardNumber, expiryMonth, expiryYear, cvc, responseCb) {
		NativeModules.PaystackIOS.getToken(String(cardNumber), String(expiryMonth), String(expiryYear), String(cvc), responseCb);
	}
};

module.exports = PaystackIOS;