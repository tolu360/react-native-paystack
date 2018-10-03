
import React from 'react'

import { NativeModules, Platform } from 'react-native'

const { RNPaystackModule } = NativeModules;
const checkInit = (instance) => {
	if (!instance.paystackInitialized) {
		throw new Error(`You should call init first, higher up your code like in your index file.\nRead more https://github.com/tolu360/react-native-paystack#3-usage`)
	}
}

class RNPaystack {
	paystackInitialized = false;

	init(options: { [x: string]: any }) {
		if (typeof options != 'object') {
			return Promise.reject(new Error("Method argument can only be a Javascript object"));
		}
		this.paystackInitialized = true;

		return RNPaystackModule.init(options);
	}

	chargeCard(chargeParams: { [x: string]: any }) {
		if (typeof chargeParams != 'object') {
			return Promise.reject(new Error("Method argument can only be a Javascript object"));
		}
		checkInit(this);

		return RNPaystackModule.chargeCard(chargeParams);
	}

	chargeCardWithAccessCode(chargeParams: { [x: string]: any }) {
		if (typeof chargeParams != 'object') {
			return Promise.reject(new Error("Method argument can only be a Javascript object"));
		}
		checkInit(this);

		return RNPaystackModule.chargeCardWithAccessCode(chargeParams);
	}
}

export default new RNPaystack()