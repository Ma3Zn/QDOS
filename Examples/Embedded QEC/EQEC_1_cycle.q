q[2] = {2,4};

pi      =  3.141592653589793238;
mpi     = -3.141592653589793238;
pi_t    =  1.047197551196597746;
mpi_t   = -1.047197551196597746;
t_pi_t  =  2.094395102393195492;
mt_pi_t = -2.094395102393195492;

pi_2    = 1.5707963267948966192;

$ACE[2] = "./custom_gate/ACE.bin";
$ADE[2] = "./custom_gate/ADE.bin";

encoding =
{
    $RP[1](1, 2,      pi,  pi);
    $RP[1](0, 1, mt_pi_t, mpi);
    $RP[1](2, 3,    pi_t,  pi);
    $RP[1](1, 2,     mpi, mpi);
};

detection =
{
    $RP[1](1, 2,     pi,  pi);
    $RP[1](0, 1, t_pi_t,  pi);
    $RP[1](2, 3,  mpi_t, mpi);
    
    $ACE[0,1];
    
    measure[0];
};

recovery_no_error =
{
    $RP[1](0, 1, mt_pi_t, mpi);
    $RP[1](2, 3,    pi_t,  pi);
    $RP[1](1, 2,     mpi, mpi);
};

recovery_error =
{
    $ADE[0,1];

    $RZ[1](1,3, pi_2);

    $RP[1](0, 1,  mpi_t, mpi);
    $RP[1](2, 3, t_pi_t,  pi);
    $RP[1](1, 2,     pi,  pi);
};

recovery =
{
    custom_logic::check_ancilla;
};

main =
{
    encoding;
    
    $$IDLE[0](3000);

    detection;
    recovery;
};