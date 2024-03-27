const Comment = ({name, msg}) => {
    return (
        <>
            <div className="comment" style={{
                'display': 'flex',
                'flexDirection': 'column',
                'backgroundColor': 'white',
                'borderRadius': '5px',
                'padding': '1em'
            }}>
                <h5 sx={{
                    'color': 'grey'
                }}>{name}</h5>
                <p sx={{
                    'marginTop': '1em'
                }}>{msg}</p>
            </div>
        </>
    )
}

export default Comment

