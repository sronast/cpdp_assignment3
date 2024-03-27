import Button from '@mui/material/Button';
import MessageDialog from './messageDialog';
import MailDialog from './mailDialog';
import PlayDialog from './playDialog';

const User = ({user}) => {
    console.log('user is ', user)
    return (
        <div className="flex">
            <span className='flex-2'>{user.name}</span>
            <PlayDialog  className="flex-1" playWith={user.name} isPlaying={false}/>
            <MessageDialog className='flex-1' sendTo={user.name} />
            <MailDialog className='flex-1' sendTo={user.name} />
        </div>
    )
}

export default User;