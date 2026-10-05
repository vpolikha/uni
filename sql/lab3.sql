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
    ChangeDate Date,
)

GO
CREATE TRIGGER
ON Products
AFTER UPDATE
AS
BEGIN

END;
