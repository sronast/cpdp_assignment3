import User from "./user";

const UserList = ({ users }) => {
    console.log(users);
    return (
        <>
            {users.map((user) => (
                <User key={user.id} user={user} />
            ))}
        </>
    );
};

export default UserList;
