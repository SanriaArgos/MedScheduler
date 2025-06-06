"use client";

import { useState, useEffect } from "react";
import { useRouter } from 'next/navigation';
import Link from "next/link";

export default function JuniorAdminProfilePage() {
    const [loading, setLoading] = useState(true);
    const [profile, setProfile] = useState(null);
    const [error, setError] = useState("");
    const router = useRouter();

    // Состояние для формы редактирования
    const [isEditing, setIsEditing] = useState(false);
    const [lastName, setLastName] = useState("");
    const [firstName, setFirstName] = useState("");
    const [patronymic, setPatronymic] = useState("");
    const [currentPassword, setCurrentPassword] = useState("");
    const [newPassword, setNewPassword] = useState("");
    const [newPasswordRepeat, setNewPasswordRepeat] = useState("");
    const [updateError, setUpdateError] = useState("");
    const [updateSuccess, setUpdateSuccess] = useState("");

    // Информация о больнице
    const [hospital, setHospital] = useState(null);

    useEffect(() => {
        // Проверка аутентификации
        const isLoggedIn = localStorage.getItem('isLoggedIn');
        const userData = JSON.parse(localStorage.getItem('medSchedulerUser') || '{}');

        if (!isLoggedIn || !userData.userId || userData.userType !== 'junior') {
            router.push('/login');
            return;
        }

        // Здесь будет загрузка данных профиля младшего администратора
        // В реальном приложении здесь будет API-запрос для получения данных
        // На данный момент, поскольку у нас нет API для получения полного профиля,
        // мы просто используем имитацию данных профиля

        const mockProfileData = {
            userId: userData.userId,
            firstName: "Анна",
            lastName: "Смирнова",
            patronymic: "Ивановна",
            phone: userData.phone || "79001234567"
        };

        setProfile(mockProfileData);

        // Инициализация полей формы редактирования
        setLastName(mockProfileData.lastName);
        setFirstName(mockProfileData.firstName);
        setPatronymic(mockProfileData.patronymic);

        // Имитация данных о больнице
        const mockHospitalData = {
            hospitalId: 1,
            fullName: "Городская поликлиника №1",
            region: "Москва",
            settlementType: "город",
            settlementName: "Москва",
            street: "Ленина",
            house: "15"
        };

        setHospital(mockHospitalData);

        setLoading(false);
    }, [router]);

    const handleEditToggle = () => {
        setIsEditing(!isEditing);
        setCurrentPassword("");
        setNewPassword("");
        setNewPasswordRepeat("");
        setUpdateError("");
        setUpdateSuccess("");
    };

    const handleUpdateProfile = async (e) => {
        e.preventDefault();
        setUpdateError("");
        setUpdateSuccess("");

        if (!currentPassword) {
            setUpdateError("Введите текущий пароль для подтверждения изменений");
            return;
        }

        if (newPassword && newPassword !== newPasswordRepeat) {
            setUpdateError("Новые пароли не совпадают");
            return;
        }

        try {
            const updateData = {
                user_id: profile.userId,
                current_password: currentPassword,
            };

            // Добавляем только измененные поля
            if (lastName !== profile.lastName) updateData.last_name = lastName;
            if (firstName !== profile.firstName) updateData.first_name = firstName;
            if (patronymic !== profile.patronymic) updateData.patronymic = patronymic;
            if (newPassword) {
                updateData.new_password = newPassword;
                updateData.new_password_repeat = newPasswordRepeat;
            }

            // API запрос на обновление профиля
            const response = await fetch('https://api.medscheduler.ru/edit_junior_admin_profile', {
                method: 'PATCH',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify(updateData),
            });

            const data = await response.json();

            if (response.ok && data.success) {
                setUpdateSuccess("Профиль успешно обновлен");

                // Обновляем данные профиля локально
                setProfile({
                    ...profile,
                    lastName: lastName,
                    firstName: firstName,
                    patronymic: patronymic
                });

                // Сбрасываем поля паролей
                setCurrentPassword("");
                setNewPassword("");
                setNewPasswordRepeat("");

                // Выходим из режима редактирования через 2 секунды
                setTimeout(() => {
                    setIsEditing(false);
                    setUpdateSuccess("");
                }, 2000);
            } else {
                setUpdateError(data.error || "Ошибка обновления профиля");
            }
        } catch (err) {
            console.error("Profile update error:", err);
            setUpdateError("Не удалось подключиться к серверу");
        }
    };

    if (loading) {
        return (
            <div className="min-h-[calc(80vh-theme(space.36))] flex items-center justify-center">
                <div className="text-xl text-gray-600">Загрузка профиля...</div>
            </div>
        );
    }

    return (
        <div className="max-w-4xl mx-auto py-8 px-4">
            <h1 className="text-3xl font-bold text-main2 mb-8">Профиль младшего администратора</h1>

            {error && (
                <div className="bg-red-100 border border-red-400 text-red-700 px-4 py-3 rounded mb-4">
                    {error}
                </div>
            )}

            <div className="grid grid-cols-1 md:grid-cols-2 gap-6">
                <div className="bg-white shadow-md rounded-lg p-6">
                    <h2 className="text-xl font-semibold text-main2 mb-4">Личные данные</h2>

                    {!isEditing ? (
                        <div>
                            <div className="space-y-4">
                                <div>
                                    <p className="text-sm text-gray-500">Фамилия</p>
                                    <p className="text-lg">{profile.lastName}</p>
                                </div>
                                <div>
                                    <p className="text-sm text-gray-500">Имя</p>
                                    <p className="text-lg">{profile.firstName}</p>
                                </div>
                                <div>
                                    <p className="text-sm text-gray-500">Отчество</p>
                                    <p className="text-lg">{profile.patronymic || "—"}</p>
                                </div>
                                <div>
                                    <p className="text-sm text-gray-500">Телефон</p>
                                    <p className="text-lg">{profile.phone}</p>
                                </div>
                            </div>

                            <button
                                onClick={handleEditToggle}
                                className="mt-6 px-4 py-2 bg-main text-white rounded hover:bg-main2 transition-colors"
                            >
                                Редактировать профиль
                            </button>
                        </div>
                    ) : (
                        <form onSubmit={handleUpdateProfile}>
                            {updateError && (
                                <div className="bg-red-100 border border-red-400 text-red-700 px-4 py-3 rounded mb-4">
                                    {updateError}
                                </div>
                            )}

                            {updateSuccess && (
                                <div className="bg-green-100 border border-green-400 text-green-700 px-4 py-3 rounded mb-4">
                                    {updateSuccess}
                                </div>
                            )}

                            <div className="space-y-4">
                                <div>
                                    <label htmlFor="lastName" className="block text-sm font-medium text-gray-700 mb-1">
                                        Фамилия
                                    </label>
                                    <input
                                        type="text"
                                        id="lastName"
                                        value={lastName}
                                        onChange={(e) => setLastName(e.target.value)}
                                        className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                                        required
                                    />
                                </div>
                                <div>
                                    <label htmlFor="firstName" className="block text-sm font-medium text-gray-700 mb-1">
                                        Имя
                                    </label>
                                    <input
                                        type="text"
                                        id="firstName"
                                        value={firstName}
                                        onChange={(e) => setFirstName(e.target.value)}
                                        className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                                        required
                                    />
                                </div>
                                <div>
                                    <label htmlFor="patronymic" className="block text-sm font-medium text-gray-700 mb-1">
                                        Отчество (необязательно)
                                    </label>
                                    <input
                                        type="text"
                                        id="patronymic"
                                        value={patronymic}
                                        onChange={(e) => setPatronymic(e.target.value)}
                                        className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                                    />
                                </div>
                                <div>
                                    <label htmlFor="phone" className="block text-sm font-medium text-gray-700 mb-1">
                                        Телефон
                                    </label>
                                    <input
                                        type="text"
                                        id="phone"
                                        value={profile.phone}
                                        className="w-full px-3 py-2 border border-gray-300 rounded-md bg-gray-100"
                                        disabled
                                    />
                                    <p className="text-xs text-gray-500 mt-1">Телефон изменить нельзя</p>
                                </div>
                            </div>

                            <div className="mt-6 mb-6">
                                <h3 className="text-lg font-medium mb-2">Изменение пароля (необязательно)</h3>
                                <div className="space-y-4">
                                    <div>
                                        <label htmlFor="newPassword" className="block text-sm font-medium text-gray-700 mb-1">
                                            Новый пароль
                                        </label>
                                        <input
                                            type="password"
                                            id="newPassword"
                                            value={newPassword}
                                            onChange={(e) => setNewPassword(e.target.value)}
                                            className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                                        />
                                    </div>
                                    <div>
                                        <label htmlFor="newPasswordRepeat" className="block text-sm font-medium text-gray-700 mb-1">
                                            Повторите новый пароль
                                        </label>
                                        <input
                                            type="password"
                                            id="newPasswordRepeat"
                                            value={newPasswordRepeat}
                                            onChange={(e) => setNewPasswordRepeat(e.target.value)}
                                            className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                                        />
                                    </div>
                                </div>
                            </div>

                            <div className="border-t pt-4">
                                <label htmlFor="currentPassword" className="block text-sm font-medium text-gray-700 mb-1">
                                    Текущий пароль (для подтверждения изменений)
                                </label>
                                <input
                                    type="password"
                                    id="currentPassword"
                                    value={currentPassword}
                                    onChange={(e) => setCurrentPassword(e.target.value)}
                                    className="w-full px-3 py-2 border border-gray-300 rounded-md focus:outline-none focus:ring-1 focus:ring-main"
                                    required
                                />
                            </div>

                            <div className="flex gap-3 mt-6">
                                <button
                                    type="submit"
                                    className="px-4 py-2 bg-main text-white rounded hover:bg-main2 transition-colors"
                                >
                                    Сохранить изменения
                                </button>
                                <button
                                    type="button"
                                    onClick={handleEditToggle}
                                    className="px-4 py-2 bg-gray-200 text-gray-800 rounded hover:bg-gray-300 transition-colors"
                                >
                                    Отмена
                                </button>
                            </div>
                        </form>
                    )}
                </div>

                {/* Информация о больнице */}
                <div className="bg-white shadow-md rounded-lg p-6">
                    <h2 className="text-xl font-semibold text-main2 mb-4">Информация о больнице</h2>

                    {hospital ? (
                        <div className="space-y-4">
                            <div>
                                <p className="text-sm text-gray-500">Название</p>
                                <p className="text-lg">{hospital.fullName}</p>
                            </div>
                            <div>
                                <p className="text-sm text-gray-500">Регион</p>
                                <p className="text-lg">{hospital.region}</p>
                            </div>
                            <div>
                                <p className="text-sm text-gray-500">Населенный пункт</p>
                                <p className="text-lg">{hospital.settlementType} {hospital.settlementName}</p>
                            </div>
                            <div>
                                <p className="text-sm text-gray-500">Адрес</p>
                                <p className="text-lg">ул. {hospital.street}, д. {hospital.house}</p>
                            </div>
                        </div>
                    ) : (
                        <p className="text-gray-500">Информация о больнице недоступна</p>
                    )}
                </div>
            </div>

            <div className="mt-8 grid grid-cols-1 md:grid-cols-3 gap-4">
                <Link href="/junior-admin/doctors" className="bg-white p-4 shadow-md rounded-lg hover:shadow-lg transition-shadow text-center">
                    <h3 className="text-lg font-semibold text-main2 mb-2">Управление врачами</h3>
                    <p className="text-gray-600">Добавление и управление врачами</p>
                </Link>
                <Link href="/junior-admin/schedule" className="bg-white p-4 shadow-md rounded-lg hover:shadow-lg transition-shadow text-center">
                    <h3 className="text-lg font-semibold text-main2 mb-2">Управление расписанием</h3>
                    <p className="text-gray-600">Добавление слотов для записи</p>
                </Link>
                <Link href="/junior-admin/waitlist" className="bg-white p-4 shadow-md rounded-lg hover:shadow-lg transition-shadow text-center">
                    <h3 className="text-lg font-semibold text-main2 mb-2">Лист ожидания</h3>
                    <p className="text-gray-600">Просмотр листа ожидания</p>
                </Link>
            </div>
        </div>
    );
}
