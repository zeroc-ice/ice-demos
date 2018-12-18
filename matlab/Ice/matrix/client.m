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

        matrix = MatrixPrx.checkedCast(communicator.propertyToProxy('Matrix.Proxy'));
        if isempty(matrix)
            fprintf('invalid proxy\n');
            return;
        end

        % Fetch a matrix from the server
        matrixData = matrix.fetchData();

        % Convert the data into a matlab matrix
        if(matrixData.type == Demo.MatrixType.ColumnMajor)
            data = reshape(matrixData.elements, matrixData.axisLength, []);
        elseif(matrixData.type == Demo.MatrixType.RowMajor)
            % .' is shorthand for the transpose
            data = reshape(matrixData.elements, matrixData.axisLength, []).';
        end

        % Print out the dimensions and elements of the matrix
        [w, h] = size(data);
        fprintf('Received %d by %d matrix:\n', w, h);
        disp(data);

        % Compute some properties of the matrix
        sumValue = sum(data(:));
        fprintf('sum: %f\n', sumValue);
        meanValue = mean(data(:));
        fprintf('average: %f\n', meanValue);
        stdValue = std(data(:));
        fprintf('standard deviation: %f\n', stdValue);
    catch ex
        fprintf('%s\n', getReport(ex));
    end
    rmpath('generated');
end
