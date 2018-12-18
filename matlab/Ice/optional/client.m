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

        contactdb = ContactDBPrx.checkedCast(communicator.propertyToProxy('ContactDB.Proxy'));
        if isempty(contactdb)
            fprintf('invalid proxy\n');
            return;
        end

        %
        % Add a contact for "john". In this case since all parameters are provided it isn't
        % necessary to construct an Optional.
        %
        johnNumber = '123-456-7890';
        contactdb.addContact('john', NumberType.HOME, johnNumber, 0);

        fprintf('Checking john... ');

        %
        % Find the phone number for "john".
        %
        number = contactdb.queryNumber('john');

        %
        % Check if an optional value is set.
        %
        if number == Ice.Unset
            fprintf('number is incorrect ');
        end

        %
        % Check the value
        %
        if ~strcmp(number, johnNumber)
            fprintf('number is incorrect ');
        end

        % Optional can also be used in an out parameter.
        dialgroup = contactdb.queryDialgroup('john');

        if dialgroup == Ice.Unset || dialgroup ~= 0
            fprintf('dialgroup is incorrect ');
        end

        info = contactdb.query('john');

        %
        % All of the info parameters should be set.
        %
        if info.type == Ice.Unset || info.number == Ice.Unset || info.dialGroup == Ice.Unset
            fprintf('info is incorrect ');
        end
        fprintf('ok\n');

        %
        % Add a contact for "steve". The behavior of the server is
        % to default construct the Contact, and then assign all set
        % parameters. Since the default value of NumberType in the
        % slice definition is NumberType.HOME and in this case the
        % NumberType is unset it will take the default value.
        %
        steveNumber = '234-567-8901';
        contactdb.addContact('steve', Ice.Unset, steveNumber, 1);

        fprintf('Checking steve... ');
        number = contactdb.queryNumber('steve');
        if number == Ice.Unset || ~strcmp(number, steveNumber)
            fprintf('number is incorrect ');
        end

        info = contactdb.query('steve');
        %
        % Check the value for the NumberType.
        %
        if info.type == Ice.Unset || info.type ~= NumberType.HOME
            fprintf('info is incorrect ');
        end

        if ~strcmp(info.number, steveNumber) || info.dialGroup ~= 1
            fprintf('info is incorrect ');
        end

        dialgroup = contactdb.queryDialgroup('steve');
        if dialgroup == Ice.Unset || dialgroup ~= 1
            fprintf('dialgroup is incorrect ');
        end
        fprintf('ok\n');

        %
        % Add a contact for "frank". Here the dialGroup field isn't set.
        %
        frankNumber = '345-678-9012';
        contactdb.addContact('frank', NumberType.CELL, frankNumber, Ice.Unset);

        fprintf('Checking frank... ');

        number = contactdb.queryNumber('frank');
        if ~strcmp(number, frankNumber)
            fprintf('number is incorrect ');
        end
        info = contactdb.query('frank');
        %
        % The dial group field should be unset.
        %
        if info.dialGroup() ~= Ice.Unset
            fprintf('info is incorrect ');
        end

        if info.type ~= NumberType.CELL || ~strcmp(info.number, frankNumber)
            fprintf('info is incorrect ');
        end

        dialgroup = contactdb.queryDialgroup('frank');
        if dialgroup ~= Ice.Unset
            fprintf('dialgroup is incorrect ');
        end
        fprintf('ok\n');

        %
        % Add a contact for "anne". The number field isn't set.
        %
        contactdb.addContact('anne', NumberType.OFFICE, Ice.Unset, 2);

        fprintf('Checking anne... ');
        number = contactdb.queryNumber('anne');
        if number ~= Ice.Unset
            fprintf('number is incorrect ');
        end

        info = contactdb.query('anne');
        %
        % The number field should be unset.
        %
        if info.number ~= Ice.Unset
            fprintf('info is incorrect ');
        end

        if info.type ~= NumberType.OFFICE || info.dialGroup ~= 2
            fprintf('info is incorrect ');
        end

        dialgroup = contactdb.queryDialgroup('anne');
        if dialgroup ~= 2
            fprintf('dialgroup is incorrect ');
        end

        %
        % The optional fields can be used to determine what fields to
        % update on the contact. Here we update only the number for anne,
        % the remainder of the fields are unchanged.
        %
        anneNumber = '456-789-0123';
        contactdb.updateContact('anne', Ice.Unset, anneNumber, Ice.Unset);
        number = contactdb.queryNumber('anne');
        if ~strcmp(number, anneNumber)
            fprintf('number is incorrect ');
        end

        info = contactdb.query('anne');
        if ~strcmp(info.number, anneNumber) || info.type ~= NumberType.OFFICE || info.dialGroup ~= 2
            fprintf('info is incorrect ');
        end
        fprintf('ok\n');

        contactdb.shutdown();
    catch ex
        fprintf('%s\n', getReport(ex));
    end
    rmpath('generated');
end
