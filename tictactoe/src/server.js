import { Box, TextField } from '@mui/material';
import Button from '@mui/material/Button';
import { useState } from "react";

const  ServerButton = ({onConnectionChange}) => {
    const [connecting, setConnecting] = useState(false);
    const [portNumber, setPortNumber] = useState();

    const handleConnectClick = () => {
        setConnecting(!connecting);
        // if connected pass true
        onConnectionChange(true)
    };

    return (
        <>
            <Box sx={{'display' : 'flex', 'flexDirection': 'column'}}>
                <TextField variant='outlined' label="Server port number" value={portNumber} onChange={(e) => setPortNumber(e.target.value)}/>
                <Button  variant='contained'  onClick={handleConnectClick} sx={{'marginTop' : '1em'}}>
                Connect to server {portNumber}
            </Button>
            </Box>
            {connecting ? <p>Connecting</p> : null}
        </>
    );
};
export default ServerButton;