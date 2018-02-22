% **********************************************************************
% Copyright (c) 2003-2018 ZeroC, Inc. All rights reserved.
% **********************************************************************

function client()
    addpath('generated');
    if ~libisloaded('ice')
        loadlibrary('ice', @iceproto);
    end

    import Demo.*;
    import Ice.*;

    c = UI();
end
