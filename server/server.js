const express = require('express');
const cors = require('cors');

const app = express();
const port = 3000;

app.use(cors());

// Initialize Dummy Database
const DummyDatabase = [];

for (let i = 1; i <= 300; i++) {
    let nim = '135190' + ('00' + i).slice(-2);
    DummyDatabase.push({ nim: nim, amount: calculateAmount(nim), password: generatePassword(nim)});
}
for (let i = 1; i <= 300; i++) {
    let nim = '135200' + ('00' + i).slice(-2);
    DummyDatabase.push({ nim: nim, amount: calculateAmount(nim), password: generatePassword(nim)});
}
let lastTransaction = {};
function calculateAmount(nim) {
    let lastThreeDigits = nim.slice(-3);
    let amount = (parseInt(lastThreeDigits) + 100) * 1000;
    return amount;
}
function generatePassword(nim) {
    let lastFourDigits = nim.slice(-4);
    let password = '';
    for (let digit of lastFourDigits) {
        let newDigit = parseInt(digit) + 1;
        password += newDigit.toString().padStart(2, '0');
    }
    return password;
}
// Route for handling saldo requests
app.post('/saldo', (req, res) => {
    let requestData = '';
    req.on('data', chunk => {
        requestData += chunk;
    });

    req.on('end', () => {
        console.log('Received saldo request for user:', requestData);
        let parsedData = JSON.parse(requestData);
        let user = parsedData.User;

        let userIndex = DummyDatabase.findIndex(item => item.nim === user);

        let saldo = 0;
        let message = '';

        if (userIndex !== -1) {
            saldo = DummyDatabase[userIndex].amount;
            message = 'Saldo retrieved successfully';
        } else {
            message = 'User not found';
        }

        let response = { saldo: saldo, message: message };
        res.statusCode = 200;
        res.setHeader('Content-Type', 'application/json');
        res.end(JSON.stringify(response));
    });
});

// Endpoint to handle top-up
app.post('/top-up', (req, res) => {
    let requestData = '';
    req.on('data', chunk => {
        requestData += chunk;
    });

    req.on('end', () => {
        console.log('Received top-up request:', requestData);
        let parsedData = JSON.parse(requestData);
        let user = parsedData.User;
        let amount = parseInt(parsedData.Amount);

        let userIndex = DummyDatabase.findIndex(item => item.nim === user);

        if (userIndex !== -1) {
            DummyDatabase[userIndex].amount += amount;
            res.statusCode = 200;
            res.setHeader('Content-Type', 'application/json');
            res.end(JSON.stringify({ success: true, message: 'Top-up successful' }));
        } else {
            res.statusCode = 404;
            res.setHeader('Content-Type', 'application/json');
            res.end(JSON.stringify({ success: false, message: 'User not found' }));
        }
    });
});

// Route for handling transaction requests
app.post('/transaction', (req, res) => {
    let requestData = '';
    req.on('data', chunk => {
        requestData += chunk;
    });

    req.on('end', () => {
        // Handle Transaction Status Here
        console.log('Received data:', requestData);
        let parsedData = JSON.parse(requestData);
        let user = parsedData.User;
        let amount = parseInt(parsedData.Amount);

        let success = false;
        let message = '';

        // Find the user in the data array
        let userIndex = DummyDatabase.findIndex(item => item.nim === user);

        if (userIndex !== -1) {
            if (DummyDatabase[userIndex].amount >= amount) {
                // If the user has enough amount, subtract the transaction amount
                DummyDatabase[userIndex].amount -= amount;
                success = true;
                message = 'Transaction successful';
                // Update last transaction
                lastTransaction = {
                    nim: user,
                    amount: amount,
                    saldo: DummyDatabase[userIndex].amount, // Access DummyDatabase[userIndex].amount
                    success : 1,
                    status: 'Success'
                };
                console.log("Remaining Saldo : ", DummyDatabase[userIndex].amount)
            } else {
                // Update last transaction
                lastTransaction = {
                    nim: user,
                    amount: amount,
                    saldo: DummyDatabase[userIndex].amount, // Access DummyDatabase[userIndex].amount
                    success : 0,
                    status: 'Fail'
                };
                message = 'Insufficient amount';
            }
        } else {
            message = 'User not found';
        }
        let response = { success: success, message: message };
        res.statusCode = 200;
        res.setHeader('Content-Type', 'application/json');
        res.end(JSON.stringify(response));
    });
});
app.get('/saldo-list', (req, res) => {
    const saldoList = DummyDatabase.map(item => ({ nim: item.nim, amount: item.amount }));
    res.json(saldoList);
});

// Endpoint to get the last transaction
app.get('/lastTransaction', (req, res) => {
    res.statusCode = 200;
    res.setHeader('Content-Type', 'application/json');
    res.end(JSON.stringify(lastTransaction));
});


app.listen(port, () => {
    console.log(`Server running on port ${port}`);
});
