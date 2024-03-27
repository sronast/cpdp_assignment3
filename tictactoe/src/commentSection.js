import Comment from "./comment";
import { TextField } from '@mui/material';
import Button from '@mui/material/Button';
import { useState } from 'react';

const CommentSection = ({ gameNumber, comments, addComment, currentUser }) => {
    const [cmt, setCmt] = useState('');

    return (
        <>
            <div className="box" style={{
                border: '1px solid grey',
                maxHeight: '300px', // Set maximum height for scrollable area
                overflowY: 'scroll' // Enable vertical scrolling
            }}>
                <h5>
                    Comment Section for game: {gameNumber}
                </h5>
                {comments.map((c, index) => <Comment name={c.name} msg={c.msg} key={index} />)}
            </div>
            <div className="type" style={{
                display: 'flex',
                flexDirection: 'row',
                marginTop: '10px'
            }}>
                <TextField
                    autoFocus
                    required
                    margin="dense"
                    id="name"
                    name="message"
                    label="Message"
                    type="text"
                    fullWidth
                    variant="standard"
                    value={cmt}
                    onChange={(e) => setCmt(e.target.value)}
                    style={{
                        flex: 'flex-3'
                    }}
                />
                <Button
                    style={{
                        flex: 'flex-1'
                    }}
                    onClick={() => { addComment(currentUser, cmt); setCmt(''); }}>
                    Send
                </Button>
            </div>
        </>
    );
};

export default CommentSection;
