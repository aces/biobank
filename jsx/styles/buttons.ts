import styled from 'styled-components';

// Base action button with common styles
const BaseButton = styled.button<{onClick: () => void}>`
  font-size: 30px;
  color: #FFFFFF;
  border-radius: 50%;
  height: 45px;
  width: 45px;
  cursor: pointer;
  user-select: none;
  display: flex;
  justify-content: center;
  align-items: center;
  box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2), 0 6px 20px rgba(0, 0, 0, 0.19);
  &:hover {
    box-shadow: 0 6px 10px rgba(0, 0, 0, 0.2), 0 8px 22px rgba(0, 0, 0, 0.19);
  }
`;
// // Applying styles to the glyphicon inside the action buttons
// const Glyphicon = styled.span`
//   font-size: 20px;
//   position: relative;
//   top: 0;
// `;


// Specific styled action buttons with different background colors and specific glyphicons
const Add = styled(BaseButton).attrs({
  className: "glyphicon glyphicon-plus"
})`
  background-color: #0f9d58;
`;

const Duplicate = styled(BaseButton).attrs({
  className: "glyphicon glyphicon-duplicate"
})`
  background-color: #0f9d58;
`;

const Remove = styled(BaseButton).attrs({
  className: "glyphicon glyphicon-remove"
})`
  background-color: #808080;
`;

// export const RemoveItemButton = styled.span`
//   color: #808080;
//   margin-left: 10px;
//   cursor: 'pointer';
//   font-size: 15px;
// `;

const Pool = styled(BaseButton).attrs({
  className: "glyphicon glyphicon-tint"
})`
  background-color: #96398C;
`;

const Prepare = styled(BaseButton).attrs({
  className: "glyphicon glyphicon-scissors"
})`
  background-color: #A6D3F5;
`;

const Search = styled(BaseButton).attrs({
  className: "glyphicon glyphicon-search"
})`
  background-color: #E98430;
`;

const Update = styled(BaseButton).attrs({
  className: "glyphicon glyphicon-refresh"
})`
  background-color: #FFFFFF;
  color: #DDDDDD;
  border: 2px solid #DDDDDD;
  &:hover {
    border: none;
    background-color: #093782;
    color: #FFFFFF;
  }
`;

const Delete = styled(BaseButton).attrs({
  className: "glyphicon glyphicon-remove"
})`
  &:hover {
    border: none;
    background-color: #BC1616;
    color: #FFFFFF;
  }
`;

const Disabled = styled(BaseButton).attrs({
  className: "glyphicon glyphicon-ban-circle"
})`
  background-color: #dddddd;
`;

export const Button = {
  Add,
  Remove,
  Duplicate,
  Pool,
  Prepare,
  Search,
  Update,
  Delete,
  Disabled,
};
