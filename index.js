
import React from 'react'

import { NativeModules } from 'react-native'

const RNPaystackNative = NativeModules.RNPaystack

class RNPaystack {
	getToken(cardNumber, expiryMonth, expiryYear, cvc) {
		return RNPaystackNative.getToken(String(cardNumber), String(expiryMonth), String(expiryYear), String(cvc));
	}
}

export default new RNPaystack()