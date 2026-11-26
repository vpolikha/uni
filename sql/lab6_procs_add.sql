USE NORTHWND;

GO
CREATE OR ALTER PROCEDURE usp_ValidateCustomer
    @CustomerID NVARCHAR(5)
AS
BEGIN
     IF NOT EXISTS (
        SELECT 1
        FROM Customers
        WHERE CustomerID = @CustomerID
     )
     BEGIN
        RAISERROR('INVALID CUSTOMERID', 16, 1);
     END
END;

GO
CREATE OR ALTER PROCEDURE usp_ValidateStock
    @ProductID INT,
    @Quantity SMALLINT
AS
BEGIN
    IF @Quantity > (
        SELECT p.UnitsInStock
        FROM Products p
        WHERE p.ProductID = @ProductID
    )
    BEGIN
        RAISERROR('NOT ENOUGH PRODUCTS IN STOCK',16,1);
    END
END

GO
CREATE OR ALTER PROCEDURE usp_PlaceOrder
    @CustomerID NVARCHAR(5),
    @EmployeeID INT,
    @ProductID INT,
    @Quantity SMALLINT,
    @NewOrderID INT OUTPUT
AS
BEGIN
    BEGIN TRY
        EXEC usp_ValidateCustomer @CustomerID
        EXEC usp_ValidateStock @ProductID, @Quantity
        BEGIN TRANSACTION
            BEGIN TRY
                INSERT INTO Orders(CustomerID, EmployeeID, OrderDate)
                VALUES (@CustomerID, @EmployeeID, GETDATE());

                SET @NewOrderID = SCOPE_IDENTITY();

                INSERT INTO [Order Details](OrderID, ProductID, Quantity, UnitPrice, Discount)
                VALUES (@NewOrderID, @ProductID, @Quantity, 1000, 0);
                
                UPDATE Products
                SET UnitsInStock -= @Quantity
                WHERE ProductID = @ProductID;
           
                COMMIT TRANSACTION;
            END TRY
            BEGIN CATCH
                ROLLBACK TRANSACTION;
                RAISERROR('CANT UPDATE INFO', 16, 1);
            END CATCH
    END TRY
    BEGIN CATCH
        PRINT ERROR_MESSAGE()
    END CATCH
END

DECLARE @NewID INT;
EXEC usp_PlaceOrder 'ALFKI', 1, 1, 2, @NewID OUTPUT
PRINT 'NEW ID = '
PRINT @NewID

DECLARE @NewID INT;
EXEC usp_PlaceOrder 'XXXXX', 1, 1, 2, @NewID OUTPUT

DECLARE @NewID INT;
EXEC usp_PlaceOrder 'ALFKI', 1, 1, 200, @NewID OUTPUT