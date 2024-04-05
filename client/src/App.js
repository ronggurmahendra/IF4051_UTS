import React, { useState, useEffect } from 'react';
import './App.css'; // Import the CSS file

function App() {
  const [saldoList, setSaldoList] = useState([]);

  useEffect(() => {
    const fetchSaldoList = async () => {
      try {
        const response = await fetch('http://192.168.68.110:3000/saldo-list', {
          method: 'GET',
          headers: {
            'Content-Type': 'application/json',
          },
        });
        
        if (!response.ok) {
          throw new Error('Failed to fetch saldo list');
        }

        const data = await response.json();
        setSaldoList(data);
      } catch (error) {
        console.error('Error fetching saldo list:', error.message);
      }
    };

    fetchSaldoList();
  }, []);

  return (
    <div className="App">
      <header className="App-header">
        <h1>Saldo List</h1>
        <div className="table-container">
          <table>
            <thead>
              <tr>
                <th>NIM</th>
                <th>Amount</th>
              </tr>
            </thead>
            <tbody>
              {saldoList.map((saldoItem, index) => (
                <tr key={index}>
                  <td>{saldoItem.nim}</td>
                  <td>{saldoItem.amount}</td>
                </tr>
              ))}
            </tbody>
          </table>
        </div>
      </header>
    </div>
  );
}

export default App;
