q[2] = {4,4};

SWAP[2] = "./custom_gate/SWAP.bin";

qft =
{
    H[0];
    CR[1,0](2);

    H[1];

    SWAP[0,1];
};

main =
{
    qft;
};