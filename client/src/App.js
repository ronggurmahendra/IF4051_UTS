import React, { useState, useEffect } from 'react';
import { ToastContainer, toast } from 'react-toastify';
import 'react-toastify/dist/ReactToastify.css';
import './App.css'; // Import the CSS file

function formatRupiah(amount) {
  return new Intl.NumberFormat('id-ID', { style: 'currency', currency: 'IDR' }).format(amount);
}

function App() {
  const [saldoList, setSaldoList] = useState([]);
  const [nim, setNim] = useState('');
  const [amount, setAmount] = useState('');

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

  useEffect(() => {
    fetchSaldoList();
  }, []);

  const handleTopUp = async () => {
    try {
      // Make top up request
      const response = await fetch('http://192.168.68.110:3000/top-up', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({ User: nim, Amount: amount }),
      });

      if (!response.ok) {
        throw new Error('Failed to top up');
      }

      // Show success notification
      toast.success('Top up successful!', {
        position: "top-right",
        autoClose: 3000,
        hideProgressBar: false,
        closeOnClick: true,
        pauseOnHover: true,
        draggable: true,
      });

      // Refetch saldo list
      fetchSaldoList();
    } catch (error) {
      console.error('Error topping up:', error.message);
      // Show error notification
      toast.error('Failed to top up', {
        position: "top-right",
        autoClose: 3000,
        hideProgressBar: false,
        closeOnClick: true,
        pauseOnHover: true,
        draggable: true,
      });
    }
  };

  return (
    <div className="App">
      <header className="App-header">
        <h1>Saldo List</h1>
        <div>
          <input 
            type="text" 
            placeholder="NIM" 
            value={nim} 
            onChange={(e) => setNim(e.target.value)} 
          />
          <input 
            type="number" 
            placeholder="Amount" 
            value={amount} 
            onChange={(e) => setAmount(e.target.value)} 
          />
          <button onClick={handleTopUp}>Top Up</button>
        </div>
        <div className="table-container">
          <table>
            <thead>
              <tr>
                <th>NIM</th>
                <th>Amount (Rp.)</th>
              </tr>
            </thead>
            <tbody>
              {saldoList.map((saldoItem, index) => (
                <tr key={index}>
                  <td>{saldoItem.nim}</td>
                  <td>{formatRupiah(saldoItem.amount)}</td>
                </tr>
              ))}
            </tbody>
          </table>
        </div>
        <ToastContainer />
      </header>
    </div>
  );
}

export default App;