
import React from 'react'

import { NativeModules, Platform } from 'react-native'

const RNPaystackNative = NativeModules.RNPaystack

class RNPaystack {
	getToken(cardNumber, expiryMonth, expiryYear, cvc) {
		return RNPaystackNative.getToken(String(cardNumber), String(expiryMonth), String(expiryYear), String(cvc));
	}

	chargeCard(cardNumber, expiryMonth, expiryYear, cvc, email, amountInKobo) {
		if (Platform.OS != 'android') {
			return Promise.reject(new Error("Method is not supported on iOS platform yet"));
		}

		return RNPaystackNative.chargeCard(String(cardNumber), String(expiryMonth), String(expiryYear), String(cvc), String(email), parseInt(amountInKobo, 10));
	}
}

export default new RNPaystack()