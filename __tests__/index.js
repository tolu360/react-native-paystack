import RNPaystack from "..";

const PUBLIC_KEY = "pk_test_63847c1ea97fdd25178ca0297bbdb71542e1a9bb";
RNPaystack.init({ publicKey: PUBLIC_KEY});

it("Expects custom fields to be added", async () => {

    try {
        const response = await RNPaystack.chargeCard({
            cardNumber: '4123450131001381', 
            expiryMonth: '10', 
            expiryYear: '17', 
            cvc: '883',
            email: 'chargeIOS@master.dev',
            amountInKobo: 150000,
            subAccount: 'ACCT_pz61jjjsslnx1d9',
        });

        console.log("Paystack Response => ", response);
    } catch (err) {
        console.log("An error occurred => ", err);
    }

})