const http = require('http');

const hostname = '192.168.68.110'; // Change this to your server's IP address
const port = 3000;

console.log("Initialize Dummy Database")
const DummyDatabase = [];
// const data = [
//     { nim: '13519008', amount: calculateAmount('13519008') },
//     { nim: '13519009', amount: calculateAmount('13519009') },
//     { nim: '135190010', amount: calculateAmount('135190010') },
//   ];
for (let i = 1; i <= 20; i++) {
    let nim = '135190' + ('00' + i).slice(-2) ;
    DummyDatabase.push({ nim: nim, amount: calculateAmount(nim) });
}
for (let i = 1; i <= 20; i++) {
    let nim = '135200' + ('00' + i).slice(-2);
    DummyDatabase.push({ nim: nim, amount: calculateAmount(nim) });
}
console.log(DummyDatabase)

function calculateAmount(nim) {
    let lastThreeDigits = nim.slice(-3);
    let amount = (parseInt(lastThreeDigits) + 100) * 1000;
    return amount;
}
const server = http.createServer((req, res) => {
  if (req.method === 'POST' && req.url === '/transaction') {
    let requestData = '';
    req.on('data', chunk => {
      requestData += chunk;
    });

    req.on('end', () => {
    //   Handle Transaction Status Here
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
  } else if (req.method === 'POST' && req.url === '/saldo') {
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
  }else {
    res.statusCode = 404;
    res.setHeader('Content-Type', 'text/plain');
    res.end('Page not found\n');
  }
});

server.listen(port, hostname, () => {
  console.log(`Server running at http://${hostname}:${port}/`);
});
