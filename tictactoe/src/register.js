import * as React from "react";
import Button from "@mui/material/Button";
import Dialog from "@mui/material/Dialog";
import DialogActions from "@mui/material/DialogActions";
import DialogContent from "@mui/material/DialogContent";
import DialogContentText from "@mui/material/DialogContentText";
import DialogTitle from "@mui/material/DialogTitle";
import { TextField } from "@mui/material";

export default function RegisterButton() {
  const [open, setOpen] = React.useState(false);
  const [username, setUsername] = React.useState("");
  const [password, setPassword] = React.useState("");
  const [repeatPassword, setRepeatPassword] = React.useState("");
  const [onError, setOnError] = React.useState(false);

  const handleClickOpen = () => {
    setOpen(true);
    setUsername("");
    setPassword("");
    setRepeatPassword("");
    setOnError(false);
  };

  const handleClose = () => {
    setUsername("");
    setPassword("");
    setRepeatPassword("");
    setOnError(false);
    setOpen(false);
  };

  const handleRegisterClick = () => {
    if (password === repeatPassword){
        setOpen(false);
    }else{
        setOnError(true);
    }
  };

  return (
    <React.Fragment>
      <Button onClick={handleClickOpen}>Register</Button>
      <Dialog
        open={open}
        onClose={handleClose}
        aria-labelledby="alert-dialog-title"
        aria-describedby="alert-dialog-description"
      >
        <DialogTitle id="alert-dialog-title">Register</DialogTitle>
        <DialogContent sx={{ display: "flex", flexDirection: "column" }}>
          {/* <DialogContentText id="alert-dialog-description">
            The user has to accept the game in order to play. Send play request?
          </DialogContentText> */}
          <TextField
            id="outlined-basic"
            label="Username"
            variant="outlined"
            value={username}
            onChange={(e) => setUsername(e.target.value)}
          />
          <TextField
            id="outlined-password-input"
            label="Password"
            type="password"
            autoComplete="current-password"
            sx={{
              marginTop: "10px",
            }}
            value={password}
            onChange={(e) => setPassword(e.target.value)}
          />
          <TextField
            id="outlined-password-input"
            label="Repeat Password"
            type="password"
            autoComplete="current-password"
            sx={{
              marginTop: "10px",
            }}
            value={repeatPassword}
            onChange={(e) => setRepeatPassword(e.target.value)}
          />
        </DialogContent>
        {onError ? <p style={{'color': 'red', 'paddingLeft': '1em'}}>Passwords do not match</p> : <></>}
        <DialogActions>
          <Button onClick={handleClose}>Cancel</Button>
          <Button onClick={handleRegisterClick} autoFocus>
            Register
          </Button>
        </DialogActions>
      </Dialog>
    </React.Fragment>
  );
}
