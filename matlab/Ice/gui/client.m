% Copyright (c) ZeroC, Inc.

function client()
    addpath('generated');
    if ~libisloaded('ice')
        loadlibrary('ice', @iceproto);
    end

    import Demo.*;
    import Ice.*;

    UI();
end
