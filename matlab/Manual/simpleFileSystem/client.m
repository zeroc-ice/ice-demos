% **********************************************************************
% Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
% **********************************************************************

function client()
    addpath('generated');
    if ~libisloaded('ice')
        loadlibrary('ice', @iceproto);
    end

    import Filesystem.*;

    try
        % Initializes a communicator and then destroys it when cleanup is collected
        communicator = Ice.initialize();
        cleanup = onCleanup(@() communicator.destroy());

        rootDir = Filesystem.DirectoryPrx.checkedCast(communicator.stringToProxy('RootDir:default -h localhost -p 10000'));
        if isempty(rootDir)
            fprintf('invalid proxy\n');
            return;
        end

        % Recursively list the contents of the root directory
        fprintf("Contents of root directory:\n");
        listRecursive(rootDir, 0);
    catch ex
        fprintf('%s\n', getReport(ex));
    end
    rmpath('generated');
end

% Recursively print the contents of directory "dir" in tree fashion.
% For files, show the contents of each file. The "depth"
% parameter is the current nesting level (for indentation).
function listRecursive(dir, depth)
    depth = depth + 1;
    %char(9) is the encoding of a tab character
    indent = repmat(char(9), 1 ,depth);

    contents = dir.list();

    for i = contents
        node = i{1};
        subdir = Filesystem.DirectoryPrx.checkedCast(node);
        fprintf('%s%s ', indent, node.name());
        if(~isempty(subdir))
            fprintf('(directory)\n');
            listRecursive(subdir, depth);
        else
            fprintf('(file)\n');
            file = Filesystem.FilePrx.uncheckedCast(node);
            text = file.read();
            for line = text
                fprintf('%s\t%s\n', indent, line{1});
            end
        end
    end
end
