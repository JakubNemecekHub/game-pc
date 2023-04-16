#pragma once


class SerializationManager;

class Serial
{
public:
    virtual ~Serial() {}
    virtual void write(SerializationManager* io) = 0;
    // virtual void read(SerializationManager* io) = 0;
};