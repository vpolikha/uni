USE NORTHWND;
DISABLE TRIGGER ALL ON NORTHWND;
--1
GO
CREATE TRIGGER ShipCountryTrigger
ON Orders
AFTER INSERT
AS
BEGIN
 IF EXISTS(
  SELECT 1
  FROM inserted i
  WHERE i.ShipCountry is NULL
  OR i.ShipCountry = '' 
 )
 BEGIN
  PRINT 'Empty ShipCountry'
  ROLLBACK TRANSACTION
 END
END;
INSERT INTO Orders (CustomerID, EmployeeID)
VALUES ('ALFKI', 1)
--2
GO
CREATE OR ALTER TRIGGER OrderUpdateTrigger
ON Customers
AFTER UPDATE
AS
BEGIN
 IF EXISTS(
  SELECT 1
  FROM inserted i
  WHERE i.Phone is not NULL
 )
 BEGIN
  PRINT 'Nelzya'
  ROLLBACK TRANSACTION
 END
END;
UPDATE Customers
SET Phone = 000-000-0000
WHERE CustomerID = 'ALFKI'
--3
GO
CREATE OR ALTER TRIGGER FullBlockTrigger
ON DATABASE
FOR alter_table, drop_table
AS
BEGIN
 ROLLBACK TRANSACTION;
 raiserror ('nelzya', 16, 1)
 --PRINT 'Chi kareli poxel'
END;
DROP TABLE TestTable;

--4

SELECT
    *
FROM sys.triggers

--5

DISABLE TRIGGER OrderUpdateTrigger ON Customers;

--check
UPDATE Customers
SET Phone = 000-000-0000
WHERE CustomerID = 'ALFKI'

--enable back
ENABLE TRIGGER OrderUpdateTrigger ON Customers;

--6


DROP TRIGGER ShipCountryTrigger;

--check

SELECT
    *
FROM sys.triggers






--add_1

GO
CREATE OR ALTER PROCEDURE AddOrder
    @CustomerID NCHAR(5),
    @EmployeeID INT,
    @OrderDate DATETIME = NULL,
    @ShipCountry NVARCHAR(15)
AS
BEGIN
    BEGIN TRY
        INSERT INTO Orders (CustomerID, EmployeeID, OrderDate, ShipCountry)
        VALUES (@CustomerID, @EmployeeID, @OrderDate, @ShipCountry)
        PRINT 'DONE!'
    END TRY
    BEGIN CATCH
        PRINT 'ERROR: '+ ERROR_MESSAGE();
    END CATCH
END;

GO
EXEC AddOrder
    @CustomerID = 'ALFKI',
    @EmployeeID = 1,
    @ShipCountry = 'Germany'

-- add_2

DISABLE TRIGGER ALL ON DATABASE;

CREATE TABLE History_Log_1
(
    LogID INT IDENTITY(1, 1) PRIMARY KEY,
    ProductID INT NOT NULL,
    OldPrice Money,
    NewPrice Money,
    ChangeDate Date
)

GO
CREATE OR ALTER TRIGGER ProdUpdateLog
ON Products
AFTER UPDATE
AS
BEGIN
    INSERT INTO History_Log_1 (ProductID, OldPrice, NewPrice, ChangeDate)
    SELECT 
        i.ProductID,
        d.UnitPrice AS OldPrice,
        i.UnitPrice AS NewPrice,
        GETDATE() AS ChangeDate
    FROM inserted i
    INNER JOIN deleted d ON i.ProductID = d.ProductID
    WHERE i.UnitPrice <> d.UnitPrice;
END;

UPDATE Products
SET UnitPrice = 18.0
WHERE ProductID = 1

SELECT *
FROM History_Log_1

--add_3

CREATE TYPE CustomerTableType AS TABLE (
    CustomerID NCHAR(5) NOT NULL,
    CompanyName NVARCHAR(40) NOT NULL,
    ContactName NVARCHAR(30) NULL,
    ContactTitle NVARCHAR(30) NULL,
    Address NVARCHAR(60) NULL,
    City NVARCHAR(15) NULL,
    Region NVARCHAR(15) NULL,
    PostalCode NVARCHAR(10) NULL,
    Country NVARCHAR(15) NULL,
    Phone NVARCHAR(24) NULL,
    Fax NVARCHAR(24) NULL
);

GO
CREATE PROCEDURE usp_BulkInsertCustomers
    @NewCustomers CustomerTableType READONLY
AS
BEGIN
    SET NOCOUNT ON;

    INSERT INTO Customers (
        CustomerID, CompanyName, ContactName, ContactTitle, 
        Address, City, Region, PostalCode, Country, Phone, Fax
    )
    SELECT 
        CustomerID, CompanyName, ContactName, ContactTitle, 
        Address, City, Region, PostalCode, Country, Phone, Fax
    FROM @NewCustomers;
END;

DECLARE @TestCustomers AS CustomerTableType;

INSERT INTO @TestCustomers (CustomerID, CompanyName, ContactName, ContactTitle, Address, City, Region, PostalCode, Country, Phone, Fax)
VALUES 
    ('TST01', 'Acme Corporation', 'John Doe', 'Owner', '123 Main St', 'New York', 'NY', '10001', 'USA', '555-0100', '555-0101'),
    ('TST02', 'Globex Industries', 'Jane Smith', 'Manager', '456 Market St', 'San Francisco', 'CA', '94105', 'USA', '555-0200', '555-0201');

EXEC usp_BulkInsertCustomers @NewCustomers = @TestCustomers;

SELECT * 
FROM Customers 
WHERE CustomerID IN ('TST01', 'TST02');