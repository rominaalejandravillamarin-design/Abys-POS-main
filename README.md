<div align="center">

#  CajApp 🛒

### Point of Sale and Inventory System

![Language](https://img.shields.io/badge/LANGUAGE-C-blue?style=for-the-badge&logo=c)
![Platform](https://img.shields.io/badge/PLATFORM-WINDOWS-0078D6?style=for-the-badge&logo=windows)
![Status](https://img.shields.io/badge/STATUS-BETA-orange?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)

<br>

**✮ An efficient Point of Sale system developed in C, focused on inventory management, billing, and data persistence. ✮**

</div>

---

## ​💻​ Key Features

### 💡​ Inventory Management
* ✮ **Data Persistence:** Automatically loads and saves products in `inventario.txt`.
* ✮ **Stock Control:** Real-time validation of stock availability before a sale.
* ✮ **Administration:** Modules to add new products, edit prices, and manually adjust stock.

### 💰 Sales and Register System
* ✮ **Automatic Billing:** Detailed ticket generation in `ticket_CajApp.txt` with tax calculation (VAT).
* ✮ **Register Closure:** Detailed financial report (`cierre_de_caja.txt`) broken down by tax types and net total.
* ✮ **Visual Interface:** Clear menus and screen clearing for a better user experience.

---

## 📊​​ Implemented Algorithms

To meet academic standards and optimize performance, the system uses:

1. 🌸​**Bubble Sort:**
    * Used to automatically keep the inventory sorted by ID every time a new product is registered.
    
2. 🌸​**Binary Search:**
    * Implemented to instantly locate products by ID during the sales process, eliminating wait times in large catalogs.

---

## 📂 File Structure

* `main.c`: Main source code of the system.
* `inventario.txt`: Product database (Format: `ID;Name;Price;Stock;VAT`).
* `ticket_CajApp.txt`: Last generated sales ticket.
* `cierre_de_caja.txt`: Accumulated history of shift closures.

---

## 🎓 Academic Details

| Field | Information |
| :--- | :--- |
| **Institution** | Escuela Politécnica Nacional (EPN) |
| **Subject** | Programming I |
| **Degree** | Data Science and AI Engineering |
| **Developers** | **Matthew Llerena and Romina Villamarin** |

---


