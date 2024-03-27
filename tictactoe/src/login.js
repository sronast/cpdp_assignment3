import * as React from 'react';
import Button from '@mui/material/Button';
import Dialog from '@mui/material/Dialog';
import DialogActions from '@mui/material/DialogActions';
import DialogContent from '@mui/material/DialogContent';
import DialogContentText from '@mui/material/DialogContentText';
import DialogTitle from '@mui/material/DialogTitle';
import { TextField } from '@mui/material';

export default function LoginButton() {
  const [open, setOpen] = React.useState(false);

  const handleClickOpen = () => {
    setOpen(true);
  };

  const handleClose = () => {
    setOpen(false);
  };

  return (
    <React.Fragment>
      <Button onClick={handleClickOpen} >
        Login
      </Button>
      <Dialog
        open={open}
        onClose={handleClose}
        aria-labelledby="alert-dialog-title"
        aria-describedby="alert-dialog-description"
      >
        <DialogTitle id="alert-dialog-title">
          Login
        </DialogTitle>
        <DialogContent sx={{'display': 'flex', 'flexDirection': 'column'}}>
          {/* <DialogContentText id="alert-dialog-description">
            The user has to accept the game in order to play. Send play request?
          </DialogContentText> */}
          <TextField id="outlined-basic" label="Username" variant="outlined" />
          <TextField
          id="outlined-password-input"
          label="Password"
          type="password"
          autoComplete="current-password"
          sx={{
            marginTop: '10px'
          }}
        />
        </DialogContent>
        <DialogActions >
          <Button onClick={handleClose}>Cancel</Button>
          <Button onClick={handleClose} autoFocus>
            Login
          </Button>
        </DialogActions>
      </Dialog>
    </React.Fragment>
  );
}