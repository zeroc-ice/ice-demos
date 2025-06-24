function client(args)
    %CLIENT An Ice client that recursively lists directory contents from a remote filesystem server.

    % Copyright (c) ZeroC, Inc.

    arguments (Repeating)
        args (1, :) char
    end

    % Load the Ice library if it is not already loaded.
    if ~libisloaded('ice')
        loadlibrary('ice', @iceproto);
    end

    % Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
    communicator = Ice.Communicator(args);

    % Destroy the communicator when the function exits.
    cleanup = onCleanup(@() communicator.destroy());

    % Create a proxy for the root directory.
    rootDir = filesystem.DirectoryPrx(communicator, 'RootDir:tcp -h localhost -p 4061');

    % Recursively list the contents of the root directory
    fprintf("Contents of root directory:\n");
    listRecursive(rootDir, 0);
end

% Recursively print the contents of directory "dir" in tree fashion.
% For files, show the contents of each file. The "depth" parameter is the current nesting level (for indentation).
function listRecursive(dir, depth)
    depth = depth + 1;
    % char(9) is the encoding of a tab character.
    indent = repmat(char(9), 1, depth);

    contents = dir.list();

    for i = contents
        node = i{1};

        assert(~isempty(node), 'Node is empty!'); % The node proxies returned by list() are never null.

        % Check if this node is a directory by asking the remote object.
        subdir = filesystem.DirectoryPrx.checkedCast(node);

        kind = '(directory)';
        if(isempty(subdir))
            % We assume it's a file if it's not a directory.
            kind = '(file)';
        end

        fprintf('%s%s %s:\n', indent, node.name(), kind);

        if(~isempty(subdir))
            listRecursive(subdir, depth);
        else
            % Read and print the contents of the file.
            file = filesystem.FilePrx.uncheckedCast(node);
            lines = file.read();
            for j = lines
                fprintf('%s\t%s\n', indent, j{1});
            end
        end
    end
end
