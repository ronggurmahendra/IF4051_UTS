const express = require('express');
const cors = require('cors');

const app = express();
const port = 3000;

app.use(cors());

// Initialize Dummy Database
const DummyDatabase = [];

for (let i = 1; i <= 300; i++) {
    let nim = '135190' + ('00' + i).slice(-2);
    DummyDatabase.push({ nim: nim, amount: calculateAmount(nim) });
}
for (let i = 1; i <= 300; i++) {
    let nim = '135200' + ('00' + i).slice(-2);
    DummyDatabase.push({ nim: nim, amount: calculateAmount(nim) });
}

function calculateAmount(nim) {
    let lastThreeDigits = nim.slice(-3);
    let amount = (parseInt(lastThreeDigits) + 100) * 1000;
    return amount;
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
                console.log("Remaining Saldo : ", DummyDatabase[userIndex].amount)
            } else {
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
app.listen(port, () => {
    console.log(`Server running on port ${port}`);
});
