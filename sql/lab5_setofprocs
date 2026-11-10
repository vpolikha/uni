USE NORTHWND;

GO
CREATE OR ALTER PROCEDURE usp_ValidateCustomer
    @CustomerID NCHAR(5)
AS
BEGIN
    SET NOCOUNT ON;

    IF NOT EXISTS (SELECT * FROM Customers WHERE CustomerID = @CustomerID)
    BEGIN
        RAISERROR('Error, no such customer', 16, 1);
        RETURN;
    END
END;
GO



CREATE OR ALTER PROCEDURE usp_ValidateStock
    @ProductID INT,
    @Quantity SMALLINT
AS
BEGIN
    SET NOCOUNT ON;

    DECLARE @UnitsInStock SMALLINT;

    SELECT @UnitsInStock = UnitsInStock 
    FROM Products 
    WHERE ProductID = @ProductID;

    IF @UnitsInStock IS NULL
    BEGIN
        RAISERROR('No such product', 16, 1);
        RETURN;
    END

    IF @UnitsInStock < @Quantity
    BEGIN
        RAISERROR('Insufficient amount of products', 16, 1);
        RETURN;
    END
END;
GO



CREATE OR ALTER PROCEDURE usp_PlaceOrder
    @CustomerID NCHAR(5),
    @EmployeeID INT,
    @ProductID INT,
    @Quantity SMALLINT,
    @NewOrderID INT OUTPUT
AS
BEGIN
    SET NOCOUNT ON;

    BEGIN TRY
        EXEC usp_ValidateCustomer @CustomerID = @CustomerID;
        EXEC usp_ValidateStock @ProductID = @ProductID, @Quantity = @Quantity;

        BEGIN TRANSACTION;

        INSERT INTO Orders (CustomerID, EmployeeID, OrderDate)
                    VALUES (@CustomerID, @EmployeeID, GETDATE());

        DECLARE @OrderID INT = SCOPE_IDENTITY();

        DECLARE @UnitPrice MONEY;
        SELECT @UnitPrice = UnitPrice 
        FROM Products 
        WHERE ProductID = @ProductID;

        INSERT INTO [Order Details] (OrderID, ProductID, UnitPrice, Quantity, Discount)
                             VALUES (@OrderID, @ProductID, @UnitPrice, @Quantity, 0);

        UPDATE Products
        SET UnitsInStock = UnitsInStock - @Quantity
        WHERE ProductID = @ProductID;

        COMMIT TRANSACTION;

        SET @NewOrderID = @OrderID;

    END TRY
    BEGIN CATCH
        IF @@TRANCOUNT > 0
            BEGIN
                ROLLBACK TRANSACTION;
            END
        

        DECLARE @ErrMsg NVARCHAR(4000) = ERROR_MESSAGE();

        RAISERROR(@ErrMsg, 16, 1);
    END CATCH
END;
GO



DECLARE @MyOrderID INT;

EXEC usp_PlaceOrder
    @CustomerID = 'ALFKI',
    @EmployeeID = 1,
    @ProductID = 1,
    @Quantity = 2,
    @NewOrderID = @MyOrderID OUTPUT;

SELECT @MyOrderID AS CreatedOrderID;


DECLARE @MyOrderID INT;

EXEC usp_PlaceOrder
    @CustomerID = 'ABCDEFGHIJKLMNOP',
    @EmployeeID = 1,
    @ProductID = 1,
    @Quantity = 1,
    @NewOrderID = @MyOrderID OUTPUT;


DECLARE @MyOrderID INT;

EXEC usp_PlaceOrder
    @CustomerID = 'ALFKI',
    @EmployeeID = 1,
    @ProductID = 1,
    @Quantity = 10000,
    @NewOrderID = @MyOrderID OUTPUT;
