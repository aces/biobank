import React from 'react';

interface ButtonProps {
  onClick: (e: React.MouseEvent<HTMLButtonElement>) => void;
  label?: string;
  disabled?: boolean;
}

const Button: React.FC<ButtonProps> = ({ label, onClick, disabled }) => {
  return (
    <button className={'btn btn-primary'} onClick={onClick} disabled={disabled}>
      {label}
    </button>
  );
};

export default Button;
