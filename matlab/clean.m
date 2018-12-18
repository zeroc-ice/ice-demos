% **********************************************************************
% Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
% **********************************************************************

function clean()
    function r = folders(root)
        function r = exclude(name)
            r = strcmp(name, '.') | strcmp(name, '..');
        end
        all = dir(root);
        all = all([all.isdir]);
        all = all(~cellfun(@exclude, {all.name}));
        r = {all.name};
    end

    components = folders('.');
    for i = 1:length(components)
        demos = folders(components{i});
        for j = 1:length(demos)
            demoDir = fullfile(components{i}, demos{j});
            generatedDir = fullfile(demoDir, 'generated');
            fprintf('Cleaning %s... ', demoDir);
            if exist(generatedDir, 'dir') == 7
                rmdir(generatedDir, 's');
            end
            fprintf('ok\n');
        end
    end
end
