// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

//
// The main definitions for our simple Warehouse application
//
module Warehouse
{

exception OutOfStock {};

interface Item
{
    idempotent string getDescription();
    void setDescription(string newDescription);

    idempotent float getUnitPrice();
    void setUnitPrice(float newUnitPrice);

    idempotent int quantityInStock();
    void adjustStock(int value) throws OutOfStock;
};

};

