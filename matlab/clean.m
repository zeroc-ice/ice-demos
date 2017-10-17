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
            generatedDir = fullfile(components{i}, demos{j}, 'generated');
            fprintf(1, 'Cleaning %s... ', generatedDir);
            if exist(generatedDir, 'dir') == 7
                rmdir(generatedDir, 's');
            end
            fprintf(1, 'ok\n');
        end
    end
end
