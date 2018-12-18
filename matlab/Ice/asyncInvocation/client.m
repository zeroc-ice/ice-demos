% **********************************************************************
% Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
% **********************************************************************

function client()
    addpath('generated');
    if ~libisloaded('ice')
        loadlibrary('ice', @iceproto);
    end

    import Demo.*;

    try
        % Initializes a communicator and then destroys it when cleanup is collected
        communicator = Ice.initialize({'--Ice.Config=config.client'});
        cleanup = onCleanup(@() communicator.destroy());

        calculator = CalculatorPrx.checkedCast(communicator.propertyToProxy('Calculator.Proxy'));
        if isempty(calculator)
            fprintf('invalid proxy\n');
            return;
        end

        % Calculate 10 - 4 with an asynchronous call
        future = calculator.subtractAsync(10, 4);
        % 'fetchOutputs' blocks until the call is completed
        fprintf('10 minus 4 is %i\n', future.fetchOutputs());

        % Calculate 13 / 5 asynchronously
        future = calculator.divideAsync(13, 5);
        [result, remainder] = future.fetchOutputs();
        try
            fprintf('13 / 5 is %i with a remainder of %i\n', result, remainder);
        catch ex
            if isa(ex, 'Demo.DivideByZeroException')
                fprintf('You cannot divide by 0\n')
            else
                rethrow(ex);
            end
        end
        % Same with divide by 0:
        future = calculator.divideAsync(13, 0);
        try
            [result, remainder] = future.fetchOutputs();
            fprintf('13 / 5 is %i with a remainder of %i\n', result, remainder);
        catch ex
            if isa(ex, 'Demo.DivideByZeroException')
                fprintf('You cannot divide by 0\n')
            else
                rethrow(ex);
            end
        end

        try
            % Have the calculator find the hypotenuse of a triangle with side lengths of 6 and 8
            % using the Pythagorean theorem
            future1 = calculator.squareAsync(6);
            future2 = calculator.squareAsync(8);

            future = calculator.addAsync(future1.fetchOutputs(), future2.fetchOutputs());
            future = calculator.squareRootAsync(future.fetchOutputs());
            fprintf('The hypotenuse of a triangle with side lengths of 6 and 8 is %i\n', future.fetchOutputs());
        catch ex
            if isa(ex, 'Demo.NegativeRootException')
                fprintf('You cannot take the square root of negative numbers\n');
            else
                rethrow(ex);
            end
        end

        calculator.shutdown();
    catch ex
        fprintf('%s\n', getReport(ex));
    end
    rmpath('generated');
end
